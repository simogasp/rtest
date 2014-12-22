% are3PointsAligned - Given 3 2D points it returns 1 if the points are
% aligned, 0 otherwise. It checks if the angles of the triangle formed by
% the three points are less than the given angular threshold expressed in
% degrees
%
% Usage:   aligned = are3pointsAligned( A, B, C, angleThreshold )
%          aligned = are3pointsAligned( Pts, angleThreshold )
%          
%
% Arguments:
%          A  - 3x1 or 2x1 2d point (homogeneous or non-homogeneous)
%          B  - 3x1 or 2x1 2d point (homogeneous or non-homogeneous)
%          C  - 3x1 or 2x1 2d point (homogeneous or non-homogeneous)
%          or
%          Pts - 3x3 or 2x3 2d points (homogeneous or non-homogeneous)
%          angleThreshold - angular threshold in degrees
%         
% Returns:
%          Q - the 3xN 3D points
%
%
%
% Author: Simone Gasparini 
function aligned = are3pointsAligned( varargin)

[a, b, c, angleThreshold] = checkargs(varargin(:));
% deg2rad(angleThreshold)
aligned = 0;
% rad2deg( angle2Vectors( b-a, b-c) )
% rad2deg( angle2Vectors( c-a, c-b) )
% rad2deg( angle2Vectors( a-c, a-b) )

if ( abs(angle2Vectors( b-a, b-c)) < deg2rad(angleThreshold) )
%     disp(1)
%     angle2Vectors( b-a, b-c)
    aligned = 1;
elseif ( abs(angle2Vectors( c-a, c-b)) < deg2rad(angleThreshold) )
%     disp(2)
%     angle2Vectors( c-a, c-b)
    aligned = 1;
elseif (abs( angle2Vectors( a-c, a-b)) < deg2rad(angleThreshold) )
%     disp(3)
%     angle2Vectors( a-c, a-b)
    aligned = 1;
end

return



function [A, B, C, angleThreshold] = checkargs(arg)
    
if length(arg) == 4
    A = arg{1};
    B = arg{2};
    C = arg{3};
    angleThreshold = arg{4};
    
    A = fixInput(A);
    B = fixInput(B);
    C = fixInput(C);

elseif length(arg) == 2
    pts = arg{1};
    angleThreshold = arg{2};
    if ( size(pts, 2) == 3 )
        if (size(pts, 1) == 2 )
            pts(3,:)=1;
        elseif (size(pts, 1) ~= 3 )
            error('The points must be either 3x3 or 2x3');
        end
        A = pts(:,1);
        B = pts(:,2);
        C = pts(:,3);
    else
        error('three points must be provided')
    end
else
    error('Wrong number of arguments supplied');
end


function p = fixInput(P)

if ( size(P, 2) == 1 )
    if (size(P, 1) == 2 )
        P(3,1)=1;
    elseif (size(P, 1) ~= 3 )
        error('The point must be either 3x1 or 2x1');
    end
else
    error('Wrong input point');
end

p = P;
