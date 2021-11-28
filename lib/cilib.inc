%!

% This is a largely verbatim copy of Kees van der Laan's code for circle
% inversion: 230 TUGboat, Volume 31 (2010), No. 3 MAPS 40 (2009)
% Kees van der Laan, Circle Inversions; pp.9–65. http://tug.org .

% The main functions have been prefixed kvdl:
%    kvdl_li   (line inversion)
%    kvdl_lsi  (line segment inversion)
% Some changes were made, in particular to naming and small helper functions
% (amalgamated with some of my own), the logic for arc direction in kvdl_lsi,
% and a decision to base everything on the unit circle centered at 0,0.  More
% general code can be based on these using gsave/grestore.


        % point inversion
/pinv {
% x y
%==>
% Ix Iy coordinates of point after inversion in unit circle.
    0 begin
    /y exch def /x exch def 
    1.0 x dup mul y dup mul add div dup
    x mul exch y mul        % Fx Fy
    end
} def
/pinv load 0 2 dict put


        % given two points p q, return their distance
/pqdist {
% u v x y
%==>
% sqrt((u-x)^2 + (v-y)^2)
   3 -1 roll                  % u x y v
   sub exch sub pmagn
} def

        % given a point p, return its magnitude
/pmagn {
% u v
%==>
% sqrt(u^2+v^2)
   dup mul exch dup mul add sqrt
} def


        % rotate a point p over angle phi, return result
/prot
% x y phi: point and angle of rotation (counterclockwise)
%==>
% x y coordinates of point after rotation
{ 0 begin /phi exch def /y exch def /x exch def
  /xaux x phi cos mul y phi sin mul sub def /y x phi sin mul y phi cos mul add def /x xaux def
  x y
  end
} def
/prot load 0 5 dict put


        % midpoint between to points
/pqmid
% p q on stack
%==>
% 0.5p + 0.5q, the midpoint
{  exch
   4 -1 roll add .5 mul
   3 1 roll add .5 mul
}  def


        % given two points on stack defining line, draw circle that is inverse of line. 
/kvdl_li
% x1 y1 x2 y2: the points which determine the line
% mx my r: the centre and radius of the inversion circle
%==>
% mix miy ri: centre and radius of the inverted line
{  0 begin
   /eps 0.0001 def
   /y2 exch def /x2 exch def
   /y1 exch def /x1 exch def
   /angle y1 y2 sub x1 x2 sub atan def
           angle abs 90 sub abs eps abs lt
           angle abs 270 sub abs eps abs lt
           or
   {                                      % special case: vertical line
      x1 0 pinv 0 0 pqmid
      /miy exch def /mix exch def
      /ri mix miy pmagn def
   }{
      angle abs 180 sub abs eps lt
      angle abs 360 sub abs eps lt or
      angle abs eps lt or
      {                          % special case: horizontal line
         0 y1 pinv 0 0 pqmid
         /miy exch def /mix exch def
         /ri mix miy pmagn def
      }{ % general case
         angle 180 gt {/angle angle 180 sub def} if      % reduce to range 0-180
         /angle 90 angle sub def
         x1 y1 angle prot pop /lx exch def
         lx abs eps lt{(Line through origin?) print} if  % warning
         /mix 1 lx div 2 div def /miy 0 def              % center circle
         /ri miy mix pmagn def                           % radius circle
                                 % rotate back and shift
         mix miy angle neg prot
         /miy exch def
         /mix exch def
      } ifelse
   } ifelse
   mix miy ri
   end
} def
/kvdl_li load 0 16 dict put


        % given two points on stack defining line segment, draw circle-inverse of segment. 
/kvdl_lsi
% x1 y1 x2 y2: the points which determine the line
%==>
% path of inverted cirle arc
{  0 begin
   /y2 exch def /x2 exch def /y1 exch def /x1 exch def
   x1 y1 x2 y2 kvdl_li /ri exch def /miy exch def /mix exch def
   x1 y1 pinv /y1 exch def /x1 exch def
           % x1 y1 now the point inversion of x1 y1
   x2 y2 pinv /y2 exch def /x2 exch def
           % x2 y2 now the point inversion of x1 y1
   /phi1 y1 miy sub x1 mix sub atan def
   /phi2 y2 miy sub x2 mix sub atan def
         %(x1, y1)--(0, 0) right from (xm, ym)--(0, 0)?
   /nilnilphi 0 miy sub 0 mix sub atan def

   % nilnilphi is the angle (with positive x axis, origin at (mix, miy)) of the
   % inversion centre relative to the circle centre (mix, miy) derived from the
   % line.
   % Our arc should never pass through nilnilphi as the inversion
   % centre is the limit image of the line at infinity.
   % hence we go from smaller phi to larger phi (this is counterclockwise)
   % unless nilnilphi is contained in that interval.
   % fixme: do this without swap if fewer ops.

   phi2 phi1 lt
   { /phi3 phi1 def /phi1 phi2 def /phi2 phi3 def
   } if
   nilnilphi phi1 lt nilnilphi phi2 gt or
   {x1 y1 moveto newpath mix miy ri phi1 phi2 arc}
   {x2 y2 moveto newpath mix miy ri phi2 phi1 arc} ifelse
   end
} def
/kvdl_lsi load 0 20 dict put


