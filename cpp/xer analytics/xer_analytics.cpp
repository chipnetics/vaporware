#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include <ctime>
#include <stdlib.h>
#include <map>
#include <vector>
#include <string>     

#pragma warning(disable:4996)

// Prototypes
char * strsep(char **stringp, const char *delim);
double datediff(std::string start_date, std::string end_date);
void populateContainers();
void executeAnalysis();

struct xer_row
{
	char xer_filename[128];
	char recalc_date[128];
	char task_id[128];
	char proj_id[128];
	char wbs_id[128];
	char clndr_id[128];
	char phys_complete_pct[128];
	char rev_fdbk_flag[128];
	char est_wt[128];
	char lock_plan_flag[128];
	char auto_compute_act_flag[128];
	char complete_pct_type[128];
	char task_type[128];
	char duration_type[128];
	char status_code[128];
	char task_code[128];
	char task_name[128];
	char rsrc_id[128];
	char total_float_hr_cnt[128];
	char free_float_hr_cnt[128];
	char remain_drtn_hr_cnt[128];
	char act_work_qty[128];
	char remain_work_qty[128];
	char target_work_qty[128];
	char target_drtn_hr_cnt[128];
	char target_equip_qty[128];
	char act_equip_qty[128];
	char remain_equip_qty[128];
	char cstr_date[128];
	char act_start_date[128];
	char act_end_date[128];
	char late_start_date[128];
	char late_end_date[128];
	char expect_end_date[128];
	char early_start_date[128];
	char early_end_date[128];
	char restart_date[128];
	char reend_date[128];
	char target_start_date[128];
	char target_end_date[128];
	char rem_late_start_date[128];
	char rem_late_end_date[128];
	char cstr_type[128];
	char priority_type[128];
	char suspend_date[128];
	char resume_date[128];
	char float_path[128];
	char float_path_order[128];
	char guid[128];
	char tmpl_guid[128];
	char cstr_date2[128];
	char cstr_type2[128];
	char driving_path_flag[128];
	char act_this_per_work_qty[128];
	char act_this_per_equip_qty[128];
	char external_early_start_date[128];
	char external_late_end_date[128];
	char cbs_id[128];
	char pre_pess_start_date[128];
	char pre_pess_finish_date[128];
	char post_pess_start_date[128];
	char post_pess_finish_date[128];
	char create_date[128];
	char update_date[128];
	char create_user[128];
	char update_user[128];
	char location_id[128];
	char control_updates_flag[128];
}xer_data_row;

struct date_struct
{
	std::string xer_recalc;
	std::string task_name;
	std::string task_code;
	std::string start_date;
	std::string end_date;
}date_struct_target, date_struct_actual;

struct float_struct
{
	std::string xer_recalc;
	std::string task_name;
	std::string task_code;
	std::string float_total;
}float_struct_history;



std::tm make_tm(int year, int month, int day)
{
	std::tm tm = { 0 };
	tm.tm_year = year - 1900; // years count from 1900
	tm.tm_mon = month - 1;    // months count from January=0
	tm.tm_mday = day;         // days count from 1
	return tm;
}

std::map<std::string, std::vector<date_struct>> xer_target_dates;
std::map<std::string, std::vector<date_struct>> xer_actual_dates;
std::map<std::string, std::vector<float_struct>> xer_float_history;


std::map<std::string, std::string> xer_tasknames;
std::map<std::string, std::string> xer_task_ids;

// Neccessary field maps
std::map<std::string, std::string> xer_task_type;
std::map<std::string, std::string> xer_proj_id;

// Rolling History Maps
std::map<std::string, std::vector<std::string>> history_map_pct_completion;
std::map<std::string, std::vector<std::string>> history_map_remain_float;


int main()
{
	//populate maps and structs
	populateContainers();
	
	//prelim analysis
	executeAnalysis();

	return 1;
}

void populateContainers()
{
	const char* filename;

	FILE *fp;

	filename = "tsv/TASK_combined.tsv";

	fp = fopen(filename, "rt");

	if (fp == NULL) {
		fprintf(stderr, "Unable to open %s\n", filename);
		fclose(fp);
	}

	char buffer[4096] = { '\0' };
	char discard[128] = { '\0' };

	unsigned int j = 0;
	unsigned int i = 0;


	char * token;
	char* s;
	int col_index = 0;

	while (fgets(buffer, 4096, fp))
	{
		s = strdup(buffer);
		token = strsep(&s, "\t");

		if (j > 0) // Skip header
		{
			while (token != NULL)
			{
				switch (col_index)
				{
				case 0: strcpy(xer_data_row.xer_filename, token); break;
				case 1: strcpy(xer_data_row.recalc_date, token); break;
				case 3: strcpy(xer_data_row.task_id, token); break;
				case 4: strcpy(xer_data_row.proj_id, token); break;
				case 5: strcpy(xer_data_row.wbs_id, token); break;
				case 6: strcpy(xer_data_row.clndr_id, token); break;
				case 7: strcpy(xer_data_row.phys_complete_pct, token); break;
				case 8: strcpy(xer_data_row.rev_fdbk_flag, token); break;
				case 9: strcpy(xer_data_row.est_wt, token); break;
				case 10: strcpy(xer_data_row.lock_plan_flag, token); break;
				case 11: strcpy(xer_data_row.auto_compute_act_flag, token); break;
				case 12: strcpy(xer_data_row.complete_pct_type, token); break;
				case 13: strcpy(xer_data_row.task_type, token); break;
				case 14: strcpy(xer_data_row.duration_type, token); break;
				case 15: strcpy(xer_data_row.status_code, token); break;
				case 16: strcpy(xer_data_row.task_code, token); break;
				case 17: strcpy(xer_data_row.task_name, token); break;
				case 18: strcpy(xer_data_row.rsrc_id, token); break;
				case 19: strcpy(xer_data_row.total_float_hr_cnt, token); break;
				case 20: strcpy(xer_data_row.free_float_hr_cnt, token); break;
				case 21: strcpy(xer_data_row.remain_drtn_hr_cnt, token); break;
				case 22: strcpy(xer_data_row.act_work_qty, token); break;
				case 23: strcpy(xer_data_row.remain_work_qty, token); break;
				case 24: strcpy(xer_data_row.target_work_qty, token); break;
				case 25: strcpy(xer_data_row.target_drtn_hr_cnt, token); break;
				case 26: strcpy(xer_data_row.target_equip_qty, token); break;
				case 27: strcpy(xer_data_row.act_equip_qty, token); break;
				case 28: strcpy(xer_data_row.remain_equip_qty, token); break;
				case 29: strcpy(xer_data_row.cstr_date, token); break;
				case 30: strcpy(xer_data_row.act_start_date, token); break;
				case 31: strcpy(xer_data_row.act_end_date, token); break;
				case 32: strcpy(xer_data_row.late_start_date, token); break;
				case 33: strcpy(xer_data_row.late_end_date, token); break;
				case 34: strcpy(xer_data_row.expect_end_date, token); break;
				case 35: strcpy(xer_data_row.early_start_date, token); break;
				case 36: strcpy(xer_data_row.early_end_date, token); break;
				case 37: strcpy(xer_data_row.restart_date, token); break;
				case 38: strcpy(xer_data_row.reend_date, token); break;
				case 39: strcpy(xer_data_row.target_start_date, token); break;
				case 40: strcpy(xer_data_row.target_end_date, token); break;
				case 41: strcpy(xer_data_row.rem_late_start_date, token); break;
				case 42: strcpy(xer_data_row.rem_late_end_date, token); break;
				case 43: strcpy(xer_data_row.cstr_type, token); break;
				case 44: strcpy(xer_data_row.priority_type, token); break;
				case 45: strcpy(xer_data_row.suspend_date, token); break;
				case 46: strcpy(xer_data_row.resume_date, token); break;
				case 47: strcpy(xer_data_row.float_path, token); break;
				case 48: strcpy(xer_data_row.float_path_order, token); break;
				case 49: strcpy(xer_data_row.guid, token); break;
				case 50: strcpy(xer_data_row.tmpl_guid, token); break;
				case 51: strcpy(xer_data_row.cstr_date2, token); break;
				case 52: strcpy(xer_data_row.cstr_type2, token); break;
				case 53: strcpy(xer_data_row.driving_path_flag, token); break;
				case 54: strcpy(xer_data_row.act_this_per_work_qty, token); break;
				case 55: strcpy(xer_data_row.act_this_per_equip_qty, token); break;
				case 56: strcpy(xer_data_row.external_early_start_date, token); break;
				case 57: strcpy(xer_data_row.external_late_end_date, token); break;
				case 58: strcpy(xer_data_row.cbs_id, token); break;
				case 59: strcpy(xer_data_row.pre_pess_start_date, token); break;
				case 60: strcpy(xer_data_row.pre_pess_finish_date, token); break;
				case 61: strcpy(xer_data_row.post_pess_start_date, token); break;
				case 62: strcpy(xer_data_row.post_pess_finish_date, token); break;
				case 63: strcpy(xer_data_row.create_date, token); break;
				case 64: strcpy(xer_data_row.update_date, token); break;
				case 65: strcpy(xer_data_row.create_user, token); break;
				case 66: strcpy(xer_data_row.update_user, token); break;
				case 67: strcpy(xer_data_row.location_id, token); break;
				case 68: strcpy(xer_data_row.control_updates_flag, token); break;
				}

				token = strsep(&s, "\t");
				col_index++;
			}

			//date_struct date_struct_target;
			date_struct_target.xer_recalc = xer_data_row.recalc_date;
			date_struct_target.task_name = xer_data_row.task_name;
			date_struct_target.task_code = xer_data_row.task_code;
			date_struct_target.start_date = xer_data_row.target_start_date;
			date_struct_target.end_date = xer_data_row.target_end_date;
			 
			//date_struct date_struct_actual;
			date_struct_actual.xer_recalc = xer_data_row.recalc_date;
			date_struct_actual.task_name = xer_data_row.task_name;
			date_struct_actual.task_code = xer_data_row.task_code;
			date_struct_actual.start_date = xer_data_row.act_start_date;
			date_struct_actual.end_date = xer_data_row.act_end_date;

			//float history struct
			float_struct_history.xer_recalc = xer_data_row.recalc_date;
			float_struct_history.task_name = xer_data_row.task_name;
			float_struct_history.task_code = xer_data_row.task_code;
			float_struct_history.float_total = xer_data_row.total_float_hr_cnt;

			xer_target_dates[xer_data_row.task_code].push_back(date_struct_target);
			xer_actual_dates[xer_data_row.task_code].push_back(date_struct_actual);
			xer_float_history[xer_data_row.task_code].push_back(float_struct_history);

			xer_tasknames[xer_data_row.task_code] = xer_data_row.task_name;
			xer_task_ids[xer_data_row.task_code] = xer_data_row.task_id;

			// Neccessary XER column maps
			xer_task_type[xer_data_row.task_code] = xer_data_row.task_type;
			xer_proj_id[xer_data_row.task_code] = xer_data_row.proj_id;


			// Populate History Maps
			history_map_pct_completion[xer_data_row.task_code].push_back(xer_data_row.phys_complete_pct);
			history_map_remain_float[xer_data_row.task_code].push_back(xer_data_row.total_float_hr_cnt);


		}
		j++;
		col_index = 0;


	}

	fclose(fp);
}

void executeAnalysis()
{
	FILE *output_file_analytics = fopen("xer_analytics_output.tsv", "w");
	if (output_file_analytics == nullptr)
	{
		fprintf(stderr, "Could not open xer_analytics_output.tsv - check if open on computer.");
		return;
	}

	FILE *output_file_movement_hist = fopen("xer_output_file_movement_hist.tsv", "w");
	if (output_file_movement_hist == nullptr)
	{
		fprintf(stderr, "Could not open xer_output_file_movement_hist.tsv - check if open on computer.");
		return;
	}

	FILE *output_file_float_hist = fopen("xer_output_file_float_hist.tsv", "w");
	if (output_file_float_hist == nullptr)
	{
		fprintf(stderr, "Could not open xer_output_file_float_hist.tsv - check if open on computer.");
		return;
	}

	fprintf(output_file_analytics, "%s\n", "PROJ_ID\tTASK_ID\tTASK_CODE\tTASK_NAME\tTASK_TYPE\tIS_ACTIVE\tCURRENT_DURATION\tCURRENT_START\tCURRENT_FINISH\tLAST_DURATION\tDURATION_GROWTH\tLAST_START\tLAST_FINISH\tINI_DURATION\tINI_START\tINI_FINISH\tDAYS_START_DELTA_LW\tDAYS_END_DELTA_LW\tHAS_DELTA_LW_MVMT\tDAYS_START_DELTA_OV\tDAYS_END_DELTA_OV\tSTART_DELTA_HIST\tSTART_SLIP_FREQ\tEND_DELTA_HIST\tEND_SLIP_FREQ\tPROGRESS_HIST\tIS_COMPLETED\tJUST_COMPLETED\tREALIZED_DURATION\tREALIZED_ACCURACY\tFLOAT_TOTAL\tFLOAT_HIST");
	fprintf(output_file_movement_hist, "%s\n", "TASK_CODE\tTASK_NAME\tDATE_TYPE\tRECALC_DATE\tDELTA");
	fprintf(output_file_float_hist, "%s\n", "TASK_CODE\tTASK_NAME\tDATE_TYPE\tRECALC_DATE\tDELTA");

	char output_line[8192] = { '\0' };
	char start_delta_history[4096] = { '\0' };
	char end_delta_history[4096] = { '\0' };
	int start_slips_observed = 0;
	int start_dates_observed = 0;
	int end_slips_observed = 0;
	int end_dates_observed = 0;
	double start_slip_ratio = 0.0f;
	double end_slip_ratio = 0.0f;
	
	double current_duration = 0.0f;
	double last_duration = 0.0f;
	double ini_duration = 0.0f;
	double realized_duration = 0.0f;
	double realized_accuracy = 0.0f;
	double duration_growth = 0.0f;

	int dates_recorded = 0;
	bool is_active = false;

	auto it = xer_target_dates.begin();

	char start_curr[128] = { '\0' };
	char start_lw[128] = { '\0' };
	char start_ini[128] = { '\0' };
	char end_curr[128] = { '\0' };
	char end_lw[128] = { '\0' };
	char end_ini[128] = { '\0' };
	char start_temp_2[128] = { '\0' };
	char start_temp_1[128] = { '\0' };
	char end_temp_2[128] = { '\0' };
	char end_temp_1[128] = { '\0' };

	int start_curr_d, start_curr_m, start_curr_y;
	int start_lw_d, start_lw_m, start_lw_y;
	int start_ini_d, start_ini_m, start_ini_y;
	int start_temp1_d, start_temp1_m, start_temp1_y;
	int start_temp2_d, start_temp2_m, start_temp2_y;
	int end_curr_d, end_curr_m, end_curr_y;
	int end_lw_d, end_lw_m, end_lw_y;
	int end_ini_d, end_ini_m, end_ini_y;
	int end_temp1_d, end_temp1_m, end_temp1_y;
	int end_temp2_d, end_temp2_m, end_temp2_y;

	double start_slip_lw;
	double end_slip_lw;
	double start_slip_ov;
	double end_slip_ov;
	double start_temp_delta;
	double end_temp_delta;

	char c_start_slip_lw[128] = { '\0' };
	char c_end_slip_lw[128] = { '\0' };
	char c_start_slip_ov[128] = { '\0' };
	char c_end_slip_ov[128] = { '\0' };
	char c_start_temp_wtw[128] = { '\0' };
	char c_end_temp_wtw[128] = { '\0' };
	char c_start_temp_delta[128] = { '\0' };
	char c_end_temp_delta[128] = { '\0' };

	bool has_snapshot_movement = false;

	struct history_struct
	{
		std::string xer_recalc;
		std::string task_name;
		std::string task_code;
	};

	std::vector<std::pair<history_struct, double>> start_history_delta_vect;
	std::vector<std::pair<history_struct, double>> end_history_delta_vect;

	std::tm tm_start_curr, tm_start_lw, tm_start_ini, tm_end_curr, tm_end_lw, tm_end_ini, tm_start_temp1, tm_start_temp2, tm_end_temp1, tm_end_temp2;
	
	time_t now = time(0);
	tm * tm_now = localtime(&now);

	const int seconds_per_day = 60 * 60 * 24;

	char progress_history[1024] = { '\0' };
	char float_history[1024] = { '\0' };
	char is_completed_this_snapshot[16] = "No";
	char is_completed[16] = "No";


	//VARS FOR MEAN AND VARIANCE HISTORIES
	float start_total_x = 0.0, start_total_x2 = 0.0, start_total_xy = 0.0, start_total_y = 0.0;
	float start_mean = 0.0, start_sum_diff_mean = 0.0, start_variance = 0.0;
	int start_elems{ 0 };

	float end_total_x = 0.0, end_total_x2 = 0.0, end_total_xy = 0.0, end_total_y = 0.0;
	float end_mean = 0.0, end_sum_diff_mean = 0.0, end_variance = 0.0;
	int end_elems{ 0 };

	float start_lr_coeff_a = 0.0, start_lr_coeff_b = 0.0;
	float end_lr_coeff_a = 0.0, end_lr_coeff_b = 0.0;

	float next_start_lr_predict = 0.0;
	float next_end_lr_predict = 0.0;

	
	// For each TASK ID....
	while (it != xer_target_dates.end())
	{
		dates_recorded = it->second.size();
		if (dates_recorded < 2)
		{
			it++;
			continue;
		}

		
		strcpy(start_curr, it->second[dates_recorded - 1].start_date.c_str()); // Start Current
		strcpy(start_lw, it->second[dates_recorded - 2].start_date.c_str()); // Start LW
		strcpy(start_ini, it->second[0].start_date.c_str()); // Start Initial
		strcpy(end_curr, it->second[dates_recorded - 1].end_date.c_str()); // End Current
		strcpy(end_lw, it->second[dates_recorded - 2].end_date.c_str()); // End LW
		strcpy(end_ini, it->second[0].end_date.c_str()); // End Initial

		sscanf(start_curr, "%d-%d-%d", &start_curr_y, &start_curr_m, &start_curr_d);
		sscanf(start_lw, "%d-%d-%d", &start_lw_y, &start_lw_m, &start_lw_d);
		sscanf(start_ini, "%d-%d-%d", &start_ini_y, &start_ini_m, &start_ini_d);
		sscanf(end_curr, "%d-%d-%d", &end_curr_y, &end_curr_m, &end_curr_d);
		sscanf(end_lw, "%d-%d-%d", &end_lw_y, &end_lw_m, &end_lw_d);
		sscanf(end_ini, "%d-%d-%d", &end_ini_y, &end_ini_m, &end_ini_d);

		tm_start_curr = make_tm(start_curr_y, start_curr_m, start_curr_d);
		tm_start_lw = make_tm(start_lw_y, start_lw_m, start_lw_d);
		tm_start_ini = make_tm(start_ini_y, start_ini_m, start_ini_d);
		tm_end_curr = make_tm(end_curr_y, end_curr_m, end_curr_d);
		tm_end_lw = make_tm(end_lw_y, end_lw_m, end_lw_d);
		tm_end_ini = make_tm(end_ini_y, end_ini_m, end_ini_d);

		std::time_t time_now = std::mktime(tm_now);
		std::time_t time_start_curr = std::mktime(&tm_start_curr);
		std::time_t time_start_lw = std::mktime(&tm_start_lw);
		std::time_t time_start_ini = std::mktime(&tm_start_ini);
		std::time_t time_end_curr = std::mktime(&tm_end_curr);
		std::time_t time_end_lw = std::mktime(&tm_end_lw);
		std::time_t time_end_ini = std::mktime(&tm_end_ini);

		start_slip_lw = std::difftime(time_start_curr, time_start_lw) / seconds_per_day; // LW Start Slip
		end_slip_lw = std::difftime(time_end_curr, time_end_lw) / seconds_per_day; // LW End Slip
		start_slip_ov = std::difftime(time_start_curr, time_start_ini) / seconds_per_day; // Overall Start Slip
		end_slip_ov = std::difftime(time_end_curr, time_end_ini) / seconds_per_day; // Overall Start Slip

		if (std::difftime(time_start_curr, time_now) <0 && std::difftime(time_end_curr, time_now) >0)
			is_active = true;
		else
			is_active = false;

		// DURATIONS
		current_duration = std::difftime(time_end_curr, time_start_curr) / seconds_per_day;
		last_duration = std::difftime(time_end_lw, time_start_lw) / seconds_per_day;
		ini_duration = std::difftime(time_end_ini, time_start_ini) / seconds_per_day;

		if (ini_duration != 0)
			duration_growth = ((double)current_duration / (double)ini_duration)-1.00f;
		else
			duration_growth = 0;

		if (start_slip_lw != 0 || end_slip_lw != 0)
			has_snapshot_movement = true;
		else
			has_snapshot_movement = false;

		sprintf(c_start_slip_lw, "%0.1lf", start_slip_lw);
		sprintf(c_end_slip_lw, "%0.1lf", end_slip_lw);
		sprintf(c_start_slip_ov, "%0.1lf", start_slip_ov);
		sprintf(c_end_slip_ov, "%0.1lf", end_slip_ov);


		// Start and End Delta History
		for (int q = dates_recorded; q >= 2; q--)
		{
			sscanf(it->second[q - 1].start_date.c_str(), "%d-%d-%d", &start_temp1_y, &start_temp1_m, &start_temp1_d);
			sscanf(it->second[q - 2].start_date.c_str(), "%d-%d-%d", &start_temp2_y, &start_temp2_m, &start_temp2_d);
			sscanf(it->second[q - 1].end_date.c_str(), "%d-%d-%d", &end_temp1_y, &end_temp1_m, &end_temp1_d);
			sscanf(it->second[q - 2].end_date.c_str(), "%d-%d-%d", &end_temp2_y, &end_temp2_m, &end_temp2_d);
			tm_start_temp1 = make_tm(start_temp1_y, start_temp1_m, start_temp1_d);
			tm_start_temp2 = make_tm(start_temp2_y, start_temp2_m, start_temp2_d);
			tm_end_temp1 = make_tm(end_temp1_y, end_temp1_m, end_temp1_d);
			tm_end_temp2 = make_tm(end_temp2_y, end_temp2_m, end_temp2_d);
			std::time_t time_tm_start_temp1 = std::mktime(&tm_start_temp1);
			std::time_t time_tm_start_temp2 = std::mktime(&tm_start_temp2);
			std::time_t time_tm_end_temp1 = std::mktime(&tm_end_temp1);
			std::time_t time_tm_end_temp2 = std::mktime(&tm_end_temp2);

			start_temp_delta = std::difftime(time_tm_start_temp1, time_tm_start_temp2) / seconds_per_day;
			end_temp_delta = std::difftime(time_tm_end_temp1, time_tm_end_temp2) / seconds_per_day;
			
			sprintf(c_start_temp_delta, "%0.1lf", start_temp_delta);
			sprintf(c_end_temp_delta, "%0.1lf", end_temp_delta);

			if (start_temp_delta != 0) start_slips_observed++;

			if (end_temp_delta != 0) end_slips_observed++;

			start_dates_observed++;
			end_dates_observed++;

			strcat(start_delta_history, c_start_temp_delta);
			strcat(start_delta_history, ",");
			strcat(end_delta_history, c_end_temp_delta);
			strcat(end_delta_history, ",");

			// Load vectors with history
			history_struct temp_hist;
			temp_hist.task_code = it->second[q - 1].task_code;
			temp_hist.task_name = it->second[q - 1].task_name;
			temp_hist.xer_recalc = it->second[q - 1].xer_recalc;

			start_history_delta_vect.push_back(std::make_pair(temp_hist, start_temp_delta));
			end_history_delta_vect.push_back(std::make_pair(temp_hist, end_temp_delta));
		}

		/// Print this history of the activity to the output.

		for (int i = 0; i < start_history_delta_vect.size();i++)
			fprintf(output_file_movement_hist, "%s\t%s\t%s\t%s\t%0.2lf\n", start_history_delta_vect[i].first.task_code.c_str(), start_history_delta_vect[i].first.task_name.c_str(), "Target Start", start_history_delta_vect[i].first.xer_recalc.c_str(), start_history_delta_vect[i].second);
		for (int i = 0; i < end_history_delta_vect.size(); i++)
			fprintf(output_file_movement_hist, "%s\t%s\t%s\t%s\t%0.2lf\n", end_history_delta_vect[i].first.task_code.c_str(), end_history_delta_vect[i].first.task_name.c_str(), "Target End", end_history_delta_vect[i].first.xer_recalc.c_str(), end_history_delta_vect[i].second);

		// End printing

		start_slip_ratio = (double)start_slips_observed / (double)start_dates_observed;
		end_slip_ratio = (double)end_slips_observed / (double)end_dates_observed;

		// Float History
		for (int q = history_map_remain_float[it->first.c_str()].size(); q >= 1; q--)
		{
			strcat(float_history, history_map_remain_float[it->first.c_str()].at(q - 1).c_str());
			strcat(float_history, ",");
		}

		///Print the History of the float
		for (int i = 0; i < xer_float_history[it->first.c_str()].size(); i++)
		{
			fprintf(output_file_float_hist, "%s\t%s\t%s\t%s\t%s\n",
				xer_float_history[it->first.c_str()].at(i).task_code.c_str(),
				xer_float_history[it->first.c_str()].at(i).task_name.c_str(),
				"Float Total",
				xer_float_history[it->first.c_str()].at(i).xer_recalc.c_str(),
				xer_float_history[it->first.c_str()].at(i).float_total.c_str());
		}

		//Delete trailing comma
		if (strlen(float_history)>1)
			float_history[strlen(float_history) - 1] = '\0';

		for (int q = history_map_pct_completion[it->first.c_str()].size(); q >= 1; q--)
		{
			strcat(progress_history, history_map_pct_completion[it->first.c_str()].at(q - 1).c_str());
			strcat(progress_history, ",");
		}
		//Delete trailing comma
		if (strlen(progress_history)>1)
			progress_history[strlen(progress_history) - 1] = '\0';


		// Completed in Snapshot (period) Flag
		int r = history_map_pct_completion[it->first.c_str()].size();


		if (strcmp(history_map_pct_completion[it->first.c_str()].at(r - 1).c_str(), "100") == 0)
		{
			strcpy(is_completed, "Yes");
			realized_duration = abs(datediff(xer_actual_dates[it->first.c_str()].back().end_date, xer_actual_dates[it->first.c_str()].back().start_date));
			if (ini_duration != 0)
				realized_accuracy = (double)realized_duration / (double)ini_duration;
			else
				realized_accuracy = 0.0f;

			if (strcmp(history_map_pct_completion[it->first.c_str()].at(r - 2).c_str(), "100") != 0)
			{
				strcpy(is_completed_this_snapshot, "Yes");
			}
			else
				strcpy(is_completed_this_snapshot, "No");
		}
		else
		{
			strcpy(is_completed, "No");
			strcpy(is_completed_this_snapshot, "No");
		}

		//Delete trailing commas from history
		if (strlen(start_delta_history)>1)
			start_delta_history[strlen(start_delta_history) - 1] = '\0';
		if (strlen(end_delta_history)>1)
			end_delta_history[strlen(end_delta_history) - 1] = '\0';

		// Iterate vector in reverse.
		/*
		for (auto it_start_hist = start_history_delta_vect.rbegin(); it_start_hist != start_history_delta_vect.rend(); ++it_start_hist)
		{
			start_elems++;
			start_total_x = start_total_x + start_elems;
			start_total_x2 = start_total_x2 + start_elems*start_elems;
			start_total_y = start_total_y + it_start_hist->first[1];
			start_total_xy = start_total_xy + start_elems*(it_start_hist->first[1]);
			
		}
		
		for (auto it_end_hist = end_history_delta_vect.rbegin(); it_end_hist != end_history_delta_vect.rend(); ++it_end_hist)
		{
			end_elems++;
			end_total_x = end_total_x + end_elems;
			end_total_x2 = end_total_x2 + end_elems*end_elems;
			end_total_y = end_total_y + it_end_hist->first[1];
			end_total_xy = end_total_xy + end_elems*(it_end_hist->first[1]);
		}
		*/

		//Means
		start_mean = start_total_y / start_elems;
		end_mean = end_total_y / end_elems;

		//Variances
		//for (auto& n : start_history_delta_vect)
		//	start_sum_diff_mean += pow(n - start_mean,2);
		//for (auto& n : end_history_delta_vect)
		//	end_sum_diff_mean += pow(n - end_mean, 2);

		start_variance = start_sum_diff_mean / start_elems;
		end_variance = end_sum_diff_mean / end_elems;

		//Coeffs
		start_lr_coeff_b = ((start_elems*start_total_xy) - (start_total_x*start_total_y)) /(start_elems*start_total_x2 - (start_total_x*start_total_x));
		start_lr_coeff_a = (start_total_y - start_lr_coeff_b*start_total_x) /start_elems;

		
		end_lr_coeff_b = ((end_elems*end_total_xy) - (end_total_x*end_total_y)) /(end_elems*end_total_x2 - (end_total_x*end_total_x));
		end_lr_coeff_a = (end_total_y - (end_lr_coeff_b*end_total_x)) / end_elems;

		next_start_lr_predict = start_lr_coeff_a + start_lr_coeff_b*(start_elems + 1);
		next_end_lr_predict = end_lr_coeff_a + end_lr_coeff_b*(end_elems + 1);

		// Reset vars
		start_total_x = 0;start_total_x2 = 0;start_total_xy = 0;start_total_y = 0;
		start_elems = 0; start_sum_diff_mean = 0;
		end_total_x = 0; end_total_x2 = 0; end_total_xy = 0; end_total_y = 0;
		end_elems = 0; end_sum_diff_mean = 0;
		
		// Output file writing.
		strcat(output_line, xer_proj_id[it->first.c_str()].c_str());  // PROJ ID
		strcat(output_line, "\t");
		strcat(output_line, xer_task_ids[it->first.c_str()].c_str());  // TASK ID
		strcat(output_line, "\t");
		strcat(output_line, it->first.c_str());  // WBS
		strcat(output_line, "\t");
		strcat(output_line, xer_tasknames[it->first.c_str()].c_str());  // TASK NAME
		strcat(output_line, "\t");
		strcat(output_line, xer_task_type[it->first.c_str()].c_str());  // TASK Type
		strcat(output_line, "\t");
		strcat(output_line, std::to_string(is_active).c_str());  // Is Active
		strcat(output_line, "\t");
		strcat(output_line, std::to_string(current_duration).c_str());  // Duation
		strcat(output_line, "\t");
		strcat(output_line, start_curr);  // Current Start
		strcat(output_line, "\t");
		strcat(output_line, end_curr); // Current Finish
		strcat(output_line, "\t");
		strcat(output_line, std::to_string(last_duration).c_str());  // Last duration
		strcat(output_line, "\t");
		strcat(output_line, std::to_string(duration_growth).c_str());  // Duration Growth
		strcat(output_line, "\t");
		strcat(output_line, start_lw);  // LW Start
		strcat(output_line, "\t");
		strcat(output_line, end_lw);  // LW Finish
		strcat(output_line, "\t");
		strcat(output_line, std::to_string(ini_duration).c_str());  // Initial duration
		strcat(output_line, "\t");
		strcat(output_line, start_ini);  // Initial Start
		strcat(output_line, "\t");
		strcat(output_line, end_ini);  // Initial Finish
		strcat(output_line, "\t");
		strcat(output_line, c_start_slip_lw);  // Days Start Slippage LW
		strcat(output_line, "\t");
		strcat(output_line, c_end_slip_lw);  // Days End Slippage LW
		strcat(output_line, "\t");
		strcat(output_line, std::to_string(has_snapshot_movement).c_str());  // Has Snapshot Movemnt
		strcat(output_line, "\t");
		strcat(output_line, c_start_slip_ov);  // Days Start Slippage Overall
		strcat(output_line, "\t");
		strcat(output_line, c_end_slip_ov);  // Days End Slippage Overall
		strcat(output_line, "\t");
		strcat(output_line, start_delta_history);  // Start Delta History
		strcat(output_line, "\t");
		strcat(output_line, std::to_string(start_slip_ratio).c_str());  // Start Slip Ratio
		strcat(output_line, "\t");
		strcat(output_line, end_delta_history);  // End Delta History
		strcat(output_line, "\t");
		strcat(output_line, std::to_string(end_slip_ratio).c_str());  // End Slip Ratio
		strcat(output_line, "\t");
		strcat(output_line, progress_history); // Progress History
		strcat(output_line, "\t");
		strcat(output_line, is_completed); // Is Completed in General
		strcat(output_line, "\t");
		strcat(output_line, is_completed_this_snapshot); // Is Completed in this latest Pass
		strcat(output_line, "\t");
		strcat(output_line, std::to_string(realized_duration).c_str());  // Realized duration
		strcat(output_line, "\t");
		strcat(output_line, std::to_string(realized_accuracy).c_str());  // Realized accuracy (planning accuracy)
		strcat(output_line, "\t");
		strcat(output_line, history_map_remain_float[it->first.c_str()].back().c_str());  // total float
		strcat(output_line, "\t");
		strcat(output_line, float_history); // Float history
		//strcat(output_line, "\t");
		//strcat(output_line, std::to_string(start_mean).c_str());  // Start Mean
		//strcat(output_line, "\t");
		//strcat(output_line, std::to_string(start_variance).c_str());  // Start Variance
		//strcat(output_line, "\t");
		//strcat(output_line, std::to_string(start_lr_coeff_a).c_str());
		//strcat(output_line, "\t");
		//strcat(output_line, std::to_string(start_lr_coeff_b).c_str());
		//strcat(output_line, "\t");
		//strcat(output_line, std::to_string(next_start_lr_predict).c_str());
		//strcat(output_line, "\t");
		//strcat(output_line, std::to_string(end_mean).c_str());  // End Mean
		//strcat(output_line, "\t");
		//strcat(output_line, std::to_string(end_variance).c_str());  // End Variance
		//strcat(output_line, "\t");
		//strcat(output_line, std::to_string(end_lr_coeff_a).c_str());
		//strcat(output_line, "\t");
		//strcat(output_line, std::to_string(end_lr_coeff_b).c_str());
		//strcat(output_line, "\t");
		//strcat(output_line, std::to_string(next_end_lr_predict).c_str());
		//strcat(output_line, "\t");

		fprintf(output_file_analytics, "%s\n", output_line);


		// VARIABLE RESETS
		start_history_delta_vect.clear();
		end_history_delta_vect.clear();

		memset(output_line, '\0', 1024);
		memset(start_delta_history, '\0', 1024);
		memset(end_delta_history, '\0', 1024);
		memset(progress_history,'\0', 1024);
		memset(float_history, '\0', 1024);

		start_slips_observed = 0;
		start_dates_observed = 0;
		end_slips_observed = 0;
		end_dates_observed = 0;
		start_slip_ratio = 0.0f;
		end_slip_ratio = 0.0f;
		
		current_duration = 0.0f;
		last_duration = 0.0f;
		ini_duration = 0.0f;
		realized_duration = 0.0f;
		realized_accuracy = 0.0f;
		duration_growth = 0.0f;


		start_total_x = 0.0, start_total_x2 = 0.0, start_total_xy = 0.0, start_total_y = 0.0;
		start_mean = 0.0, start_sum_diff_mean = 0.0, start_variance = 0.0;
		start_elems = 0;

		end_total_x = 0.0, end_total_x2 = 0.0, end_total_xy = 0.0, end_total_y = 0.0;
		end_mean = 0.0, end_sum_diff_mean = 0.0, end_variance = 0.0;
		end_elems = 0;

		start_lr_coeff_a = 0.0, start_lr_coeff_b = 0.0;
		end_lr_coeff_a = 0.0, end_lr_coeff_b = 0.0;

		next_start_lr_predict = 0.0;
		next_end_lr_predict = 0.0;


		it++;
	}


	fclose(output_file_movement_hist);
	fclose(output_file_float_hist);
	fclose(output_file_analytics);
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
