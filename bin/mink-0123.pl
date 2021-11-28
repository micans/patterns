#!/usr/bin/perl

# 0 : right
# 1 : up
# 2 : left
# 3 : down


use strict;
use warnings;

my $n = shift;
die "Refuse" unless $n < 11;

my $start = shift || die "Need start (suggest 0123 or 3210)";
my %map = ( 0 => '030', 1 => '101', 2 => '212', 3 => '323' );

sub mink {
  my $R = shift;
  my $string = shift;

  if (!$R) { return $string; }
  else {
    return join "", map { mink($R-1, $map{$_}) } (split //, $string);
  }
}

my $mink = mink($n, $start);

sub mul {
  my $mx = shift;
  my $veclist = shift;

  my @res = ();
  for my $v (@$veclist) {
    push @res,
    [  $mx->[0][0] * $v->[0] + $mx->[0][1] * $v->[1]
    ,  $mx->[1][0] * $v->[0] + $mx->[1][1] * $v->[1]
    ]
  }
  return \@res;
}

my @mx = ( [2, 1], [-1, 2] );
my $pq = [ [-1, -1] ];

while ($n > 0) {
  $pq = mul(\@mx, $pq);
  $n--;
}

my ($x, $y) = @{$pq->[0]};
my $i = 0;

for my $guide (split "", $mink) {
  my $dist = sqrt(($x**2 + $y**2)) / 2;
  my $taxi = (abs($x) + abs($y)) / 2;
  printf "%d\t%d\t%d\t%.3f\t%d\n", $x, $y, $taxi, $dist, $i;
  $x += 2 if $guide eq '0';
  $y += 2 if $guide eq '1';
  $x -= 2 if $guide eq '2';
  $y -= 2 if $guide eq '3';
  $i++;
}

print "$mink\n";

