#copyright by Eric Lee, 25/03/2017
my ($dir,$fname);

# check usage
if (@ARGV != 2) {
  print "usage: $0 dir_to_wave trainfilename\n\n"; 
  exit (0);
}

# read in command line arguments
($dir,$fname) = @ARGV;

open (FNAME,">$fname") || die ("Unable to open $fname file for writing");

#write to the file

opendir(DIR, $dir);
@files = grep(/\.mfc$/,readdir(DIR));
closedir(DIR);


use Cwd;
$dire = getcwd;


foreach $file (@files) {
	printf(FNAME "%s/%s\n",$dir,$file);
}

close(FNAME);

