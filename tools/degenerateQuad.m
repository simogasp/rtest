function d = degenerateQuad( quad, degThresh )
%DEGENERATEQUAD Return true if the quad is degeneate
%   Check whether the quad is degenerate, ie any of three vertices of the
%   quad are collinear
% Usage:   d = degenerateQuad( quad, degThresh )
%          
%
% Arguments:
%          quad  - the 4x2 vertex coordinates of the quad
%          degThresh  -  the angular threshold (in degrees) to use when checking
%          collinearity
%         

% Returns:
%           d  - 1 if any of the 4 subset of the vertices are collinear

d = any([are3pointsAligned(quad(1,:)', quad(2,:)', quad(3,:)', degThresh)...
                are3pointsAligned(quad(2,:)', quad(3,:)', quad(4,:)', degThresh)...
                are3pointsAligned(quad(3,:)', quad(4,:)', quad(1,:)', degThresh)...
                are3pointsAligned(quad(4,:)', quad(1,:)', quad(2,:)', degThresh)]);


end

