#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <vector>
#include <map>

#pragma warning(disable:4996)

#define BUFFER_SIZE 4096

char * strsep(char **stringp, const char *delim);
void getFilesList(std::string filePath, std::string extension, std::vector<std::string> & returnFileName);


int main(int argc, char * argv[])
{
	std::vector<std::string> xer_files;
	std::map<std::string, std::string> map_xer_recalc_dates;

	getFilesList("xer/", "*.xer", xer_files);

	char file_out_individual[128] = { '\0' };
	char file_out_combined[128] = { '\0' };

	FILE *fp_in;
	FILE *fp_out_TASK_combined;

	FILE *fp_out_XERFILELIST;

	// P6 nomenclature
	FILE *fp_out_ACTVTYPE;
	FILE *fp_out_TASKACTV;
	FILE *fp_out_ACTVCODE;
	FILE *fp_out_PROJECT;
	FILE *fp_out_TASKPRED;

	char buffer[BUFFER_SIZE] = { '\0' };

	fp_out_TASK_combined = fopen("tsv/TASK_combined.tsv", "wb");
	fp_out_XERFILELIST = fopen("tsv/XERFILELIST.tsv", "wb");
	
	fprintf(fp_out_XERFILELIST, "%s", "XER_file\n");


	for (size_t i = 0; i < xer_files.size(); i++)
	{
		//(keep re-writing until last file, will be master)
		fp_out_ACTVTYPE = fopen("tsv/ACTVTYPE.tsv", "wb");
		fp_out_TASKACTV = fopen("tsv/TASKACTV.tsv", "wb");
		fp_out_ACTVCODE = fopen("tsv/ACTVCODE.tsv", "wb");
		fp_out_PROJECT = fopen("tsv/PROJECT.tsv", "wb");
		fp_out_TASKPRED = fopen("tsv/TASKPRED.tsv", "wb");

		//Record filename in XERFILELIST
		fprintf(fp_out_XERFILELIST, "%s\n", xer_files.at(i).c_str());

		fp_in = fopen(xer_files.at(i).c_str(), "rb"); // read binary (rt 'read text' will terminate early in some cases with XER jargon).

		if (fp_in == NULL) {
			fprintf(stderr, "Unable to open %s\n", xer_files.at(i).c_str());
			fclose(fp_in);
		}

		int line = 0;
		while (fgets(buffer, BUFFER_SIZE, fp_in) != NULL)
		{
			// PROJECT for Data Date (last_recalc_date)
			if (strcmp(buffer, "%T\tPROJECT\r\n") == 0 || strcmp(buffer, "%T\tPROJECT\n") == 0)
			{
				char * token;
				char* s;
				int col_index = 0;

				char proj_id[128] = { '\0' };
				char recalc_date[128] = { '\0' };

				while (fgets(buffer, BUFFER_SIZE, fp_in))
				{
					if (strncmp(buffer, "%F", 2) == 0 || strncmp(buffer, "%R", 2) == 0)
					{
						fprintf(fp_out_PROJECT, "%s", buffer);

						// Skip over header
						if (strncmp(buffer, "%F", 2) == 0)
							continue;

						// Project record settings
						if (strncmp(buffer, "%R", 2) == 0)
						{
							s = strdup(buffer);
							token = strsep(&s, "\t");


							while (token != NULL)
							{
								switch (col_index)
								{
								case 1: strcpy(proj_id, token); break;
								case 28: strcpy(recalc_date, token); break;
								}

								token = strsep(&s, "\t");
								col_index++;
							}

							map_xer_recalc_dates[proj_id] = recalc_date;
							col_index = 0;
						}


					}
					else
						break;
				}
				}


				// TASKS
				if (strcmp(buffer, "%T\tTASK\r\n") == 0 || strcmp(buffer, "%T\tTASK\n") == 0)
				{
					char discard_1[128] = { '\0' };
					char discard_2[128] = { '\0' };
					char xer_proj_id[128] = { '\0' };

					while (fgets(buffer, BUFFER_SIZE, fp_in))
					{
						if (strncmp(buffer, "%F", 2) == 0 || strncmp(buffer, "%R", 2) == 0)
						{

							// On combined file, skip over header if not the first file.
							if (strncmp(buffer, "%F", 2) == 0 && i != 0)
								continue;

							// On combined file, prepend header with "XER_file" and "recalc_date"
							if (strncmp(buffer, "%F", 2) == 0 && i == 0)
							{
								fprintf(fp_out_TASK_combined, "xer_file\t");
								fprintf(fp_out_TASK_combined, "recalc_date\t");
								fprintf(fp_out_TASK_combined, "%s", buffer);
								continue;
							}

							if (strncmp(buffer, "%R", 2) == 0)
							{

								// Get the project ID from this row
								sscanf(buffer, "%s\t%s\t%s", discard_1, discard_2, xer_proj_id);


								fprintf(fp_out_TASK_combined, "%s\t", xer_files.at(i).c_str());
								fprintf(fp_out_TASK_combined, "%s\t", map_xer_recalc_dates[xer_proj_id].c_str());
								fprintf(fp_out_TASK_combined, "%s", buffer);
							}
						}
						else
							break;
					}
				}
				// ACTVTYPE
				if (strcmp(buffer, "%T\tACTVTYPE\r\n") == 0 || strcmp(buffer, "%T\tACTVTYPE\n") == 0)
				{
					while (fgets(buffer, BUFFER_SIZE, fp_in))
					{
						if (strncmp(buffer, "%F", 2) == 0 || strncmp(buffer, "%R", 2) == 0)
						{
							fprintf(fp_out_ACTVTYPE, "%s", buffer);
						}
						else
							break;
					}
				}
				// ACTVCODE
				if (strcmp(buffer, "%T\tACTVCODE\r\n") == 0 || strcmp(buffer, "%T\tACTVCODE\n") == 0)
				{
					while (fgets(buffer, BUFFER_SIZE, fp_in))
					{
						if (strncmp(buffer, "%F", 2) == 0 || strncmp(buffer, "%R", 2) == 0)
						{
							fprintf(fp_out_ACTVCODE, "%s", buffer);
						}
						else
							break;
					}
				}
				//TASKACTV
				if (strcmp(buffer, "%T\tTASKACTV\r\n") == 0 || strcmp(buffer, "%T\tTASKACTV\n") == 0)
				{
					while (fgets(buffer, BUFFER_SIZE, fp_in))
					{
						if (strncmp(buffer, "%F", 2) == 0 || strncmp(buffer, "%R", 2) == 0)
						{
							fprintf(fp_out_TASKACTV, "%s", buffer);
						}
						else
							break;
					}
				}
				
				//TASKACTV
				if (strcmp(buffer, "%T\tTASKPRED\r\n") == 0 || strcmp(buffer, "%T\tTASKPRED\n") == 0)
				{
					while (fgets(buffer, BUFFER_SIZE, fp_in))
					{
						if (strncmp(buffer, "%F", 2) == 0 || strncmp(buffer, "%R", 2) == 0)
						{
							fprintf(fp_out_TASKPRED, "%s", buffer);
						}
						else
							break;
					}
				}

			}

			memset(file_out_individual, '\0', 128);

			map_xer_recalc_dates.clear();

			fclose(fp_in);
			fclose(fp_out_ACTVTYPE);
			fclose(fp_out_TASKACTV);
			fclose(fp_out_ACTVCODE);
			fclose(fp_out_PROJECT);
			fclose(fp_out_TASKPRED);
		}
		fclose(fp_out_TASK_combined);
		return 1;
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

void getFilesList(std::string filePath, std::string extension, std::vector<std::string> & returnFileName)
{
	WIN32_FIND_DATA fileInfo;
	HANDLE hFind;
	std::string  fullPath = filePath + extension;
	hFind = FindFirstFile(fullPath.c_str(), &fileInfo);
	if (hFind != INVALID_HANDLE_VALUE){
		returnFileName.push_back(filePath + fileInfo.cFileName);
		while (FindNextFile(hFind, &fileInfo) != 0){
			returnFileName.push_back(filePath + fileInfo.cFileName);
		}
	}
}