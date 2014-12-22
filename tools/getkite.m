function kite = getkite( dt, eq, listOfEdges )
%GETKITE get the kite sorrounding a given edge
%   return the ID of the vertices forming the kite of a given edge
% Usage:   kite = getkite( dt, e )
%          
%
% Arguments:
%          dt  - delaunay triangulation
%          eq  -  an edge of the triangulation or a quad. 
%                An edge is an 1x2 or 2x1 array of two indices of the vertices of the edge
%                A quad is a 1x4 or 4x1 array of vertex IDs composing the
%                quad
%         

% Returns:
%           kite  - An array of n vertex IDs forming the kite


if((size(eq,1)==1 && size(eq,2)==2) || (size(eq,2)==1 && size(eq,1)==2))

    quadID  = getQuad( dt, eq );
    
elseif((size(eq,1)==1 && size(eq,2)==4) || (size(eq,2)==1 && size(eq,1)==4))
    
    quadID  = eq;

else
    
    error('wrong input format for edge or quad')
end

kite = [];
        
% for each vertex
for v = quadID'
    
    % get all the connected vertices that are not in the quad or
    % are not already processed
    [r, ~] = find( listOfEdges==v );               % get all the edges containing the examined vertex
    kite = [ kite; unique(reshape( listOfEdges(r,:), [], 1)) ];     % put them in a unique list
    
end

% get rid of the quadID in kyte
kite = setdiff(kite, quadID);

end

