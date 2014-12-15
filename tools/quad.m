numIter=100;

[X, Y] = meshgrid(1:10, 1:10);

X = [reshape(X, [], 1) reshape(Y, [], 1)] + 0.25*randn(100,2);

dt = DelaunayTri(X);

e = edges(dt);
numEdges = size(e,1);

figure(1)
plot(X(:,1)', X(:,2), '.')

figure(2)
triplot(dt)

% LUT = zeros(301*301, size(e,1));


uniQuad = [0,0; 0,1; 1,1; 1,0 ];

%%
LUT=[];
% for each edge
for i = 1:size(e,1)
    
    % get the quad using edgeAttachments
    % t contains the indices of the triangles in dt.Triangulation
    t = edgeAttachments(dt, e(i,1), e(i,2));
   
    
    % if t has 2 triangles
    if ( size(t{1},2) == 2 )
        
        % put the vertices in a (unique) list
        % FIXME preserve the order!
        l = reshape(dt.Triangulation(t{1},:).', [],1);
        [~, m1, ~] = unique(l);
        quadID = l(sort(m1));
        
        quad = X(quadID,:);
        
        kyte = [];
        
        % for each vertex
        for v = quadID'
        
            % get all the connected vertices that are not in the quad or
            % are not already processed 
            [r, ~] = find( e==v );               % get all the edges containing the examined vertex
            kyte = [ kyte; unique(reshape( e(r,:), [], 1)) ];     % put them in a unique list
                       
        end
        
        % get rid of the quadID in kyte
        kyte = setdiff(kyte, quadID);
        
        % now algorithm 2        
        for k = 1:numIter
            
            % add gaussian noise to the quad
            q = quad + 0.1*randn(4,2);
%             q = quad;
            
            % compute homography H
            H = homography2d(q', uniQuad');
            
            for v = kyte'
               
                % compute the V = H*v                
                V = H*[X(v,:)';1];
                V = V./V(3);
                
                % quantize V
                V = round(V*10);
                % update the LUT
                if ~(isempty(LUT))
                    
                    [ism,b]=ismember(V(1:2)', LUT(:,1:2),'rows' );
                    if(ism)
                        LUT(b,2+i) = LUT(b,2+i)+1;
                    else
                        LUT = [LUT; [V(1) V(2) zeros(1, numEdges)] ];
                    end
                else
                    LUT = [V(1) V(2) zeros(1,numEdges)];
                    LUT(1,2+i) = LUT(1,2+i)+1;
                end
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
plot(X(quadID,1)', X(quadID,2)', '-k')
plot(X(quadID,1)', X(quadID,2)', '.m')

%%
plot(X(:,1)', X(:,2),'.', 'MarkerSize', 28)



%% quering
numOK = 0;
numNOK = 0;

for i = 1:size(e,1)
    
    % get the quad using edgeAttachments
    % t contains the indices of the triangles in dt.Triangulation
    t = edgeAttachments(dt, e(i,1), e(i,2));
   
    
    % if t has 2 triangles
    if ( size(t{1},2) == 2 )
        
        % put the vertices in a (unique) list
        % FIXME preserve the order!
        l = reshape(dt.Triangulation(t{1},:).', [],1);
        [~, m1, ~] = unique(l);
        quadID = l(sort(m1));
        
        quad = X(quadID,:);
        
        kyte = [];
        
        % for each vertex
        for v = quadID'
        
            % get all the connected vertices that are not in the quad or
            % are not already processed 
            [r, ~] = find( e==v );               % get all the edges containing the examined vertex
            kyte = [ kyte; unique(reshape( e(r,:), [], 1)) ];     % put them in a unique list
                       
        end
        
        % get rid of the quadID in kyte
        kyte = setdiff(kyte, quadID);
        
        % now algorithm 1   
        H = homography2d(quad', uniQuad');
        votes = zeros(1, numEdges);
        
        % for each connected neighboor
        for v = kyte'
            V = H*[X(v,:)';1];
            V = V./V(3);
            
            % quantize V
            V = round(V*10);
            
            [ism,b]=ismember(V(1:2)', LUT(:,1:2),'rows' );
            if(ism)
                votes = votes + LUT(b,3:end);
            else
                disp('not found');
            end
            
        end
        
        % get the max votes
        [Y,I] = max(votes);
        
        if i==I
            numOK = numOK+1;
            disp(sprintf('processed edge %03d: \t got index %03d with %d votes', i, I, Y));
        else
            disp(sprintf('processed edge %03d: \t got index %03d with %d votes', i, I, Y));
            [mm, ix] = sort(votes, 'descend');
            disp([mm(1:3)' ix(1:3)'])
            numNOK = numNOK+1;
        
        end
                
    end
    
end