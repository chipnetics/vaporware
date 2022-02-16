BEGIN { 
	FS = "\t" 
}

{ 

if($1=="Project")
{
 for(i=0;i<NF;i++)
    header[i]=$i
}

if($1=="Total")
{
 for(i=0;i<NF;i++)
    footer[i]=$i
}

if($7=="CB-M002-02")
{
 for(i=18;i<NF;i++)
 {
     TRL_data[i]=$i

     gsub(",","",TRL_data[i])
     gsub("\\(","-",TRL_data[i])
     gsub("\\)","",TRL_data[i])

     TRL_data_cum[i]+=TRL_data[i]
 }
}

if(match($11,/.*Indirects.*/))
{
 for(i=18;i<NF;i++)
 {
     indirect_data[i]=$i

     gsub(",","",indirect_data[i])
     gsub("\\(","-",indirect_data[i])
     gsub("\\)","",indirect_data[i])

     indirect_data_cum[i]+=indirect_data[i]
 }
}

}

END{
 cumulative=0
 STRSEP=","

 print "Date" STRSEP\
	"Date Proper" STRSEP\
    "Date Plot" STRSEP\
	"PRJ01458 Period" STRSEP\
	"PRJ01458 Cumulative" STRSEP\
	"TRL Period" STRSEP\
	"TRL Cumulative" STRSEP\
    "Indirects Period" STRSEP\
    "Indirects Cumulative" > "tabulated.csv"

 split(header[18],a," ")

 for(i=18;i<NF;i++)
 {
    gsub(",","",footer[i])
    cumulative+=footer[i]

    TRL_data_cum_total+=TRL_data_cum[i]
    indirect_data_cum_total+=indirect_data_cum[i]

    split(header[i],a," ")

    switch(a[2])
    {
        case "Jan": 
            month = "01" 
            break
        case "Feb": 
            month = "02" 
            break
        case "Mar": 
            month = "03" 
            break
        case "Apr": 
            month = "04" 
            break
        case "May": 
            month = "05" 
            break
        case "Jun": 
            month = "06" 
            break
        case "Jul": 
            month = "07" 
            break
        case "Aug": 
            month = "08" 
            break
        case "Sep": 
            month = "09" 
            break
        case "Oct": 
            month = "10" 
            break
        case "Nov": 
            month = "11" 
            break
        case "Dec": 
            month = "12" 
            break
    }

    print header[i] STRSEP\
	a[3] "-" a[2] "-01" STRSEP\
    a[3] "-" month STRSEP\
	footer[i] STRSEP\
	cumulative STRSEP\
	TRL_data_cum[i] STRSEP\
	TRL_data_cum_total STRSEP\
    indirect_data_cum[i] STRSEP\
    indirect_data_cum_total STRSEP\
    >> "tabulated.csv"
 }
}