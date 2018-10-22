BEGIN {FS=",";count = 0}
{
	i=8;
	for i in $0 { 
		print $0[i];
	}
}
