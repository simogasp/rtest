numIter=100;

[X Y] = meshgrid(1:10, 1:10);

X = [reshape(X, [], 1) reshape(Y, [], 1)] + 0.25*randn(100,2);

dt = DelaunayTri(X);

e = edges(dt);

figure(1)
plot(X(:,1)', X(:,2), '.')

figure(2)
triplot(dt)

% for each edge
for i = 1:size(e,1)
    
    % get the quad using edgeAttachments
    % t contains the indices of the triangles in dt.Triangulation
    t = edgeAttachments(dt, e(i,1), e(i,2));
   
    
    % if t has 2 triangles
    if ( size(t{1},2) == 2 )
        
        % put the vertices in a (unique) list
        quadID = unique(reshape(dt.Triangulation(t{1},:).', [],1));
        
        quad = X(quadID,:);
        
        kyte = [];
        
        % for each vertex
        for v = quadID'
        
            % get all the connected vertices that are not in the quad or
            % are not already processed 
            [r ~] = find( e==v );               % get all the edges containing the examined vertex
            kyte = [ kyte; unique(reshape( e(r,:), [], 1)) ];     % put them in a unique list
                       
        end
        
        % get rid of the quadID in kyte
        kyte = setdiff(kyte, quadID);
        
        % now algorithm 2        
        for k = 1:numIter
            
            % add gaussian noise to the quad
            
            % compute homography H
            
            for v = kyte
               
                % compute the V = H*v
                
                %update the LUT
                
            end
            
        end
        
    end
    
end

%%
figure(2)
triplot(dt)
hold on
plot(X(e(i,:),1)', X(e(i,:),2)', '.-r')
plot(X(kyte,1)', X(kyte,2)', '.g')
plot(X(quadID,1)', X(quadID,2)', '.m')
