#!/usr/bin/perl

# On various platforms '(file) run' does not work.

use warnings;
use strict;
use Getopt::Long;

my @ARGV_COPY  = @ARGV;
my $n_args = @ARGV;

my $help     =  0;
my $descent  =  0;
my $progname = 'incexp.pl';


sub help {
   print <<EOH;
Usage:
   $progname [options]
Options:
-r              recurse
EOH
}

if
(! GetOptions
   (  "h"      =>   \$help
   ,  "r"      =>   \$descent
   )
)
   {  print STDERR "option processing failed\n";
      exit(1);
   }

if (!$n_args || $help) {
   help();
   exit 0;
}


while (<>) {
  if (/^\s*\((.*\.inc)\)\s*run/) {
     my $file = $1;
     -f $file || exit "no file $file";
     my $filename = $file;
     $filename =~ s|(.*)/|.../|;
     print STDERR "-- expanding file $file\n";
     print "% NOTE \"($filename) run\" expanded inline [\n";
     my $retval = 0;
     if ($descent) { $retval = system "incexp.pl -r $file"; }
     else {          $retval = system "cat $file"; }
     die "No descent" if $retval;
     print "% ] ($filename) run END\n";
  }
  else {
     print;
  }
}

