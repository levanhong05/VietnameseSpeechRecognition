my $output;
my $mixi;
my $split="_";
my $splas="/";
my $zero="0";
my $two="2";
$output="createGmmTrainning.bat";
$mix="..\\..\\Led\\Mix";
my $t = "t";
open(OUT, ">$output") || die ("Unable to open $output file for writing");
my $i,$j,$k;
for($i=4;$i<=64;$i=$i+2) {
	# make dir
	for($j = 0;$j<=20;$j++) {
		printf(OUT "%s\n" ,"md ..\\..\\gmm\\gmm$i\\gmm$i$split$t$j");
	}
	
	#make mixi.hed
	open(OUT2, ">$mix$i.hed") || die ("Unable to open $mixi.$i.hed file for writing");
	print(OUT2 "","MU $i {*.state[2-4].mix}");
	#make gmm
	$k = $i -2;
	print(OUT "\n","..\\..\\bin\\HHEd -A -T 1 -B -H ..\\..\\Gmm\\Gmm$k\\Gmm$k$split$t$two$zero\\macros -H ..\\..\\Gmm\\Gmm$k\\Gmm$k$split$t$two$zero\\hmmdefs -M ..\\..\\gmm\\gmm$i\\gmm$i$split$two$zero ..\\..\\Led\\mix$i.hed ..\\..\\Data\\tiedlist > ..\\..\\log\\gmm$i$split$t$two$zero");
	for($j = 1;$j<=20;$j++) {
		printf(OUT "\n");
		printf(OUT "time $splas$t");
		print(OUT "\n","..\\..\\bin\\HERest -A -T 1 -B -C ..\\..\\Config\\HERest.cfg -I ..\\..\\Mlf\\wintri.mlf -t 250.0 150.0 1000.0 -S ..\\..\\Data\\retrain.scp -H ..\\..\\gmm\\gmm$i\\gmm$i$split$t$j\\macros -H ..\\..\\gmm\\gmm$i\\gmm$i$split$t$j\\hmmdefs -M ..\\..\\gmm\\gmm$i\\gmm$i$split$t$j ..\\..\\Data\\tiedlist > ..\\..\\log\\gmm$i$split$t$j");
	}
}
