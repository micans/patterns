#!/usr/bin/perl

# On various platforms '(file) run' does not work.

use warnings;
use strict;

while (<>) {
  if (/\((.*\.inc)\)\s*run/) {
     my $file = $1;
     -f $file || exit "no file $file";
     print STDERR "-- expanding file $file\n";
     my $retval = system "cat $file";
     die "No descent" if $retval;
  }
  else {
     print;
  }
}

