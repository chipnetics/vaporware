BEGIN { 
	# Import into SQLLITE as table DATE_SNAPS
	FS = "\t" 
	print "Deliverable" "\t" "CP" "\t" "SNAP_2021_11_28" "\t" "SNAP_2021_12_26"
}
{
	hash = $2 "\t" $4
	the_map[hash]= the_map[hash] $9 "\t"  
}
END{
	for (x in the_map) 
	{
       print x "\t" the_map[x]
    }
}