% angle2Vectors - Return the angle between two 3d vectors
%
% Usage:   angle = angle = angle2Vectors( a, b)
%          
%
% Arguments:
%          a  - 3x1 vector
%          b  - 3x1 vector
%         
% Returns:
%          angle - angle between the two vectors in radiants
%
%
%
% Author: Simone Gasparini 

function angle = angle2Vectors( a, b)

if (size(a) ~= [3 1])
    error('incorrect size for the first argument')
elseif (size(b) ~= [3 1])
    error('incorrect size for the second argument')
end

angle = acos(  dot(a./norm(a), b./norm(b)) );