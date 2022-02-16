#include <stdio.h>
#include <ctime>
#include <stdlib.h>
#include <map>
#include <vector>
#include <string>     
#include <Windows.h>

#pragma warning(disable:4996)

char * strsep(char **stringp, const char *delim);
void populateContainers();
void execute_skyline();
void execute_data_structure_output();

double datediff(std::string start_date, std::string end_date);
void nearestdate(const char * start_date, int day_of_week, char * revised_date);


struct skyline_row
{
	char package_name[128];
	char current_start[128];
	char current_finish[128];
	char initial_start[128];
	char initial_finish[128];
} skyline_data_row;

std::tm make_tm(int year, int month, int day)
{
	std::tm tm = { 0 };
	tm.tm_year = year - 1900; // years count from 1900
	tm.tm_mon = month - 1;    // months count from January=0
	tm.tm_mday = day;         // days count from 1
	return tm;
};



std::map<std::string, int> map_int_package_datecnt;

std::map<std::string, std::vector<std::string>> map_vect_date_pkg_curr_finishes;
std::map<std::string, std::vector<std::string>> map_vect_date_pkg_curr_start;

std::map<std::string, std::string> map_pkg_init_finish;
std::map<std::string, std::string> map_pkg_init_start;
std::map<std::string, std::string> map_pkg_curr_finish;
std::map<std::string, std::string> map_pkg_curr_start;

int max_packages_global = 0; // The maximum packages across all dates.

int main()
{
	//populate maps and structs
	populateContainers();

	//prelim analysis
	execute_skyline();

	execute_data_structure_output();

	return 1;
}

void nearestdate(const char * start_date, int day_of_week, char * revised_date)
{
	int start_curr_y, start_curr_m, start_curr_d;

	sscanf(start_date, "%d-%d-%d", &start_curr_y, &start_curr_m, &start_curr_d);

	std::tm time_in = { 0, 0, 0, // second, minute, hour
		start_curr_d, start_curr_m - 1, start_curr_y - 1900 }; // 1-based day, 0-based month, year since 1900

	std::time_t time_start = std::mktime(&time_in);
	std::tm * time_out = std::localtime(&time_start);

	// Add offset days.
	time_out->tm_mday += (day_of_week - time_out->tm_wday);

	time_t next_date = mktime(time_out);
	std::tm * next_date_tm = std::localtime(&next_date);

	sprintf(revised_date, "%04d-%02d-%02d", next_date_tm->tm_year+1900, next_date_tm->tm_mon + 1, next_date_tm->tm_mday);
}


void populateContainers()
{
	const char* filename;

	FILE *fp;

	filename = "skyline_data_input.txt";

	fp = fopen(filename, "rt");

	if (fp == NULL) {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, 12);
		fprintf(stderr, "Unable to open %s\n", filename);
		SetConsoleTextAttribute(hConsole, 14);
		fprintf(stderr, "Closing in 5 Seconds....\n");
		_sleep(5000);
		fclose(fp);
	}

	char buffer[4096] = { '\0' };
	unsigned int input_row = 0;
	
	char * token;
	char* s;
	int col_index = 0;

	while (fgets(buffer, 4096, fp))
	{
		s = strdup(buffer);
		token = strsep(&s, "\t");

		if (input_row > 0) // Skip header
		{
			while (token != NULL)
			{
				if (token[strlen(token) - 1]=='\n')
				{
					token[strlen(token) - 1] = '\0';
				}

				switch (col_index)
				{
				case 0: strcpy(skyline_data_row.package_name, token); break;
				case 1: strcpy(skyline_data_row.current_start, token); break;
				case 2: strcpy(skyline_data_row.current_finish, token); break;
				case 3: strcpy(skyline_data_row.initial_start, token); break;
				case 4: strcpy(skyline_data_row.initial_finish, token); break;
				}

				token = strsep(&s, "\t");
				col_index++;
			}

			map_int_package_datecnt[skyline_data_row.package_name]++;

			map_pkg_init_finish[skyline_data_row.package_name] = skyline_data_row.initial_finish;
			map_pkg_init_start[skyline_data_row.package_name] = skyline_data_row.initial_start;
			map_pkg_curr_finish[skyline_data_row.package_name] = skyline_data_row.current_finish;
			map_pkg_curr_start[skyline_data_row.package_name] = skyline_data_row.current_start;

			map_vect_date_pkg_curr_finishes[skyline_data_row.current_finish].push_back(skyline_data_row.package_name);
			map_vect_date_pkg_curr_start[skyline_data_row.current_start].push_back(skyline_data_row.package_name);
		}
			
		input_row++;
		col_index = 0;
	}

	fclose(fp);

}

void execute_data_structure_output()
{
	FILE *fp = fopen("skyline_powerBI_output.tsv", "w");
	if (fp == nullptr)
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, 12);
		fprintf(stderr, "Could not open skyline_powerBI_output.tsv - check if open on computer.");
		SetConsoleTextAttribute(hConsole, 14);
		fprintf(stderr, "Closing in 5 Seconds....\n");
		_sleep(5000);
		return;
	}

	double delta = 0.0f;
	std::string s_status;
	int i_status;

	char last_week_cutoff[32] = { '\0' };
	char last_month_cutoff[32] = { '\0' };

	char reporting_week_cutoff[32] = { '\0' };
	char reporting_month_cutoff[64] = { '\0' };
	
	char year[32] = { '\0' };
	char month[32] = { '\0' };
	

	// Print header 
	fprintf(fp, "Date_Type\tData_Date\tReporting_Date\tMonth_Date\tData_Date_Index\tReporting_Date_Index\tMonth_Date_Index\tEWP\tFlag\n");
	int reporting_week_date_index = 1;
	int reporting_month_date_index = 1;
	
	for (auto it = map_vect_date_pkg_curr_finishes.begin(); it != map_vect_date_pkg_curr_finishes.end(); ++it)
	{
		for (int i = 0; i < map_vect_date_pkg_curr_finishes[it->first].size(); i++)
		{
			// Subject package
			std::string subject_package = map_vect_date_pkg_curr_finishes[it->first].at(i);

			//Get if ahead or behind, or onschedule
			delta = datediff(map_pkg_curr_finish[subject_package], map_pkg_init_finish[subject_package]);

			if (delta>0.1) { s_status = "Behind"; i_status = 0; }
			else if (delta<-0.1) { s_status = "Ahead"; i_status = 2; }
			else { s_status = "On Schedule"; i_status = 1; }

			sscanf(it->first.c_str(), "%[^-]-%[^-]", year, month);
			sprintf(reporting_month_cutoff, "%s-%s", year, month);

			nearestdate(it->first.c_str(), 6, reporting_week_cutoff);

			if (strcmp(reporting_week_cutoff, last_week_cutoff) != 0)
				reporting_week_date_index = 1;

			if (strcmp(reporting_month_cutoff, last_month_cutoff) != 0)
				reporting_month_date_index = 1;

			fprintf(fp, "%s\t%s\t%s\t%s\t%d\t%d\t%d\t%s\t%d\n", "Finish",it->first.c_str(), reporting_week_cutoff, reporting_month_cutoff, i + 1, reporting_week_date_index, reporting_month_date_index,subject_package.c_str(), i_status);

			strcpy(last_week_cutoff, reporting_week_cutoff);
			strcpy(last_month_cutoff, reporting_month_cutoff);
			reporting_week_date_index++;
			reporting_month_date_index++;
		}

	}

	reporting_week_date_index = 1;
	reporting_month_date_index = 1;

	for(auto it = map_vect_date_pkg_curr_start.begin(); it != map_vect_date_pkg_curr_start.end(); ++it)
	{
		for (int i = 0; i < map_vect_date_pkg_curr_start[it->first].size(); i++)
		{
			// Subject package
			std::string subject_package = map_vect_date_pkg_curr_start[it->first].at(i);

			//Get if ahead or behind, or onschedule
			delta = datediff(map_pkg_curr_start[subject_package], map_pkg_init_start[subject_package]);

			if (delta>0.1) { s_status = "Behind"; i_status = 0; }
			else if (delta<-0.1) { s_status = "Ahead"; i_status = 2; }
			else { s_status = "On Schedule"; i_status = 1; }

			sscanf(it->first.c_str(), "%[^-]-%[^-]", year, month);
			sprintf(reporting_month_cutoff, "%s-%s", year, month);

			nearestdate(it->first.c_str(), 6, reporting_week_cutoff);

			if (strcmp(reporting_week_cutoff, last_week_cutoff) != 0)
				reporting_week_date_index = 1;

			if (strcmp(reporting_month_cutoff, last_month_cutoff) != 0)
				reporting_month_date_index = 1;

			fprintf(fp, "%s\t%s\t%s\t%s\t%d\t%d\t%d\t%s\t%d\n", "Start", it->first.c_str(), reporting_week_cutoff, reporting_month_cutoff, i + 1, reporting_week_date_index, reporting_month_date_index, subject_package.c_str(), i_status);

			strcpy(last_week_cutoff, reporting_week_cutoff);
			strcpy(last_month_cutoff, reporting_month_cutoff);
			reporting_week_date_index++;
			reporting_month_date_index++;
		}

	}

	fclose(fp);
}

void execute_skyline()
{

	FILE *output_file_analytics = fopen("skyline_analytics_output.tsv", "w");
	if (output_file_analytics == nullptr)
	{
		fprintf(stderr, "Could not open skyline_analytics_output.tsv - check if open on computer.");
		return;
	}

	int pkg_at_date = 0;

	// Print header using map...
	for (auto it = map_vect_date_pkg_curr_finishes.begin(); it != map_vect_date_pkg_curr_finishes.end(); ++it)
	{
		fprintf(output_file_analytics, "%s\t", it->first.c_str());

		pkg_at_date = map_vect_date_pkg_curr_finishes[it->first].size();

		if (pkg_at_date > max_packages_global)
			max_packages_global = pkg_at_date;
	}

	fprintf(output_file_analytics, "\n");


	// Loop through and start listing packages....
	std::string subject_package;
	double delta = 0.0f;
	for (int output_row = 0; output_row <= max_packages_global; output_row++)
	{
		//Each loop is a date from earliest to latest....
		for (auto it = map_vect_date_pkg_curr_finishes.begin(); it != map_vect_date_pkg_curr_finishes.end(); ++it)
		{
			if (output_row >= map_vect_date_pkg_curr_finishes[it->first].size())
			{
				fprintf(output_file_analytics, "%s\t", "");
				continue;
			}
				
			// Subject package
			std::string subject_package = map_vect_date_pkg_curr_finishes[it->first].at(output_row);

			//Get if ahead or behind, or onschedule
			delta = datediff(map_pkg_curr_finish[subject_package],map_pkg_init_finish[subject_package]);

			if (delta>0.1)
				fprintf(output_file_analytics, "\"%s\n%s\"\t", subject_package.c_str(),"Behind");
			else if (delta<-0.1)
				fprintf(output_file_analytics, "\"%s\n%s\"\t", subject_package.c_str(), "Ahead");
			else
				fprintf(output_file_analytics, "\"%s\n%s\"\t", subject_package.c_str(), "On Schedule");

		}
		fprintf(output_file_analytics, "\n");
	}
}




double datediff(std::string start_date, std::string end_date)
{
	int start_curr_y, start_curr_m, start_curr_d;
	int end_curr_y, end_curr_m, end_curr_d;
	double diff;

	sscanf(start_date.c_str(), "%d-%d-%d", &start_curr_y, &start_curr_m, &start_curr_d);
	sscanf(end_date.c_str(), "%d-%d-%d", &end_curr_y, &end_curr_m, &end_curr_d);

	std::tm tm_start = make_tm(start_curr_y, start_curr_m, start_curr_d);
	std::tm tm_end = make_tm(end_curr_y, end_curr_m, end_curr_d);

	std::time_t time_tm_start = std::mktime(&tm_start);
	std::time_t time_tm_end = std::mktime(&tm_end);

	diff = std::difftime(time_tm_start, time_tm_end) / (double)(60 * 60 * 24);
	return diff;
}






char * strsep(char **stringp, const char *delim)
{
	char *s;
	const char *spanp;
	int c, sc;
	char *tok;

	if ((s = *stringp) == NULL)
		return (NULL);
	for (tok = s;;) {
		c = *s++;
		spanp = delim;
		do {
			if ((sc = *spanp++) == c) {
				if (c == 0)
					s = NULL;
				else
					s[-1] = 0;
				*stringp = s;
				return (tok);
			}
		} while (sc != 0);
	}
}