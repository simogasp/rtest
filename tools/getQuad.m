function quadID = getQuad( dt, e )
%GETQUAD return the quad associated with an edge of the Delaunay triangulation
%   return the quad associated with an edge of the Delaunay triangulation,
%   ie the two triangles containing the edge e
% Usage:   quadID  = getQuad( dt, e )
%          
%
% Arguments:
%          dt  - delaunay triangulation
%          e  -  an edge of the triangulation. An edge is an 1x2 or 2x1 array of two
%          indices of the vertices of the edge
%         

% Returns:
%           quadID  - An array of 4 vertex IDs containing the two ID of the
%           given vertices of the edge and the two vertex IDs for the other
%           two vertices associated to the edge



t = edgeAttachments(dt, e(1), e(2));

% if t has 2 triangles
if ( size(t{1},2) == 2 )
    
    % put the vertices in a (unique) list
    % FIXME preserve the order!
%     l = reshape(dt.Triangulation(t{1},:).', [],1);
%     [~, m1, ~] = unique(l);
%     quadID = l(sort(m1));
    
    l = dt.Triangulation(t{1},:);
    quadID = zeros(4,1);
    quadID(1) = e(1);
    quadID(2) = setdiff(l(1,:), e);
    quadID(3) = e(2);
    quadID(4) = setdiff(l(2,:), e);

else
    warning('only one triangle attached to this edge')
    quadID = [];
end

