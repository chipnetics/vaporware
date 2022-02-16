BEGIN
{
	
}

{
	FindAllMatches($2,/[A-Z]+[0-9]+/,an_array)

	for(i in an_array)
	{
		printf("%s", an_array[i])
		if(i != length(an_array)-1)
			printf(",");
		else
			printf("\n");
	}
}


function FindAllMatches(str, regexp, match_arr)
{

    ftotal = -1;
    ini = RSTART;
    leng = RLENGTH;

    delete match_arr;

    while (match(str, regexp) > 0)
	{
        match_arr[++ftotal] = substr(str, RSTART, RLENGTH)
        if (str == "")
			break
        str = substr(str, RSTART + (RLENGTH ? RLENGTH : 1))
    }

    RSTART = ini;
    RLENGTH = leng;
}

