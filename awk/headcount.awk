BEGIN { FS = "\t" }

{ 

if($1!="" && $1!="Card Holder")
{
     employees[$1]=$9
}

}

END{

 STRSEP=","

 print "Company" STRSEP "Headcount" > "headcount.csv"


 for(i in employees)
 {

    #print employees[i] STRSEP i
 
 }

#Unique company counts per unique employee
 for(i in employees)
 {
    companies[employees[i]]+=1
 }

 n=asorti(companies,companies_sorted)

for (i = 1; i <= n; i++) 
{
	print companies_sorted[i] STRSEP companies[companies_sorted[i]] >> "headcount.csv"
}

}


