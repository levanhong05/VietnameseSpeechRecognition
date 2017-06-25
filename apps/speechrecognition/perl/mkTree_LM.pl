#!/usr/bin/perl -w

#  generate an HHEd script file for state-clustering triphones
#   
#  mkclscript command threshold monophone_list tree
#
#  where monophone_list is a HMM list for the phones (not triphones) 
#        to be clustered
#
# Script generates a specified command and threhold for a cluster
# corresponding to each states of each phone. 
# The script assumes each model has 3 emitting states.
#
# Copyright (c) Phil Woodland, 1993, re-edited by Vo Dinh Phong, 2006


my ($threshold,$monophones,$tree);

if ( @ARGV != 3 ){
   print "usage: $0 command threshold monophones1";
   exit(1);

}
($threshold,$monophones,$tree) = @ARGV;
$command = "TB";
open (HMMLIST,"$monophones") || die ("Unable to open $monophones file for reading");
open (TREE,">$tree") || die ("Unable to open $tree file for writing");


#modify here
print TREE "RO 20 stats\n";
print TREE "TR 0\n";

#QS here
while($line = <HMMLIST>){
  chomp($line);
  print TREE "QS \"L_$line\" {$line-*}\n";
}
print TREE "QS \"L_sp\" {sp-*}\n";
close(HMMLIST);

printf TREE "\n";

open (HMMLIST,"$monophones") || die ("Unable to open $monophones file for reading");
while($line = <HMMLIST>){
  chomp($line);
  print TREE "QS \"R_$line\" {*+$line}\n";
}
print TREE "QS \"R_sp\" {*+sp}\n";
close(HMMLIST);

open (HMMLIST,"$monophones") || die ("Unable to open $monophones file for reading");

print TREE "TR 2\n";



while ($line = <HMMLIST>){
  chomp($line);
  print TREE "$command $threshold \"ST_$line";
  printf TREE "_2_\" {(\"*-$line+*\").state[2]}\n";
}
print TREE "$command $threshold \"ST_sp";
printf TREE "_2_\" {(\"*-sp+*\").state[2]}\n";
close(HMMLIST);

print TREE "\n";

open (HMMLIST,"$monophones") || die ("Unable to open $monophones file for reading");
while ($line = <HMMLIST>){
  chomp($line);
  print TREE "$command $threshold \"ST_$line";
  printf TREE "_3_\" {(\"*-$line+*\").state[3]}\n";
}
print TREE "$command $threshold \"ST_sp";
printf TREE "_3_\" {(\"*-sp+*\").state[3]}\n";
close(HMMLIST);

print TREE "\n";

open (HMMLIST,"$monophones") || die ("Unable to open $monophones file for reading");
while ($line = <HMMLIST>){
  chomp($line);
  print TREE "$command $threshold \"ST_$line";
  printf TREE "_4_\" {(\"*-$line+*\").state[4]}\n";
}
print TREE "$command $threshold \"ST_sp";
printf TREE "_4_\" {(\"*-sp+*\").state[4]}\n";

print TREE "TR 1\n";

print TREE "AU \"fulllist\"\nCO \"tiedlist\"\nST \"trees\"";

close(HMMLIST);
