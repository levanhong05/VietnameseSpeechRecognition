#copyright by Eric Lee, 23/05/2017
my ($dir,$fname);

# check usage
if (@ARGV != 2) {
  print "usage: $0 directory contains wavef files - outfilename\n\n"; 
  exit (0);
}

# read in command line arguments
($dir,$fname) = @ARGV;

open (FNAME,">$fname") || die ("Unable to open $fname file for writing");

#write to the file

$counter = 1;

opendir(DIR, $dir);
@files = grep(/\.wav$/,readdir(DIR));
closedir(DIR);

use Cwd;

foreach $file (@files) {
	$filename = substr($file,0,length($file) - 3);
	printf(FNAME "%s/%s %s/%smfc\n", $dir, $file, $dir, $filename);
}

close(FNAME);

