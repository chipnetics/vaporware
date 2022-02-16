BEGIN { FS = "\t" }

{ 

if($1!="" && $1!="Card Holder")
{
	if(match($13,/BUS.*/) && employees_entry[$1]=="")
	{
     	employees_entry[$1]=$12
     	company_name[$1]=$9
	}

    else if(match($13,/BUS.*/) && employees_entry[$1]!="")
    {
     	employees_exit[$1]=$12
    }
}

}

END{

 STRSEP=","

 print "Employee" STRSEP "Company" STRSEP "BUS In" STRSEP "BUS Out" STRSEP "Hours" > "hours.csv"


 for(i in employees_entry)
 {
    split(employees_entry[i],entry_split," "); 
    split(entry_split[2],t_entry,":"); 

    split(employees_exit[i],exit_split," "); 
    split(exit_split[2],t_exit,":"); 

	t_entry_stamp = mktime(sprintf("1990 01 01 %d %d %d 0",t_entry[1],t_entry[2],t_entry[3])); 
    t_exit_stamp = mktime(sprintf("1990 01 01 %d %d %d 0",t_exit[1],t_exit[2],t_exit[3])); 

    t_duration[i] =  (t_exit_stamp-t_entry_stamp)/60/60

    #print i STRSEP employees_entry[i] STRSEP employees_exit[i] STRSEP t_duration[i]
 }

n=asorti(employees_entry,employees_entry_sorted)

for (i = 1; i <= n; i++) 
{
	print 	employees_entry_sorted[i] STRSEP\
	        company_name[employees_entry_sorted[i]] STRSEP\
			employees_entry[employees_entry_sorted[i]] STRSEP\
			employees_exit[employees_entry_sorted[i]] STRSEP\
			t_duration[employees_entry_sorted[i]] >> "hours.csv"
}


}