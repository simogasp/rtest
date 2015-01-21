clear all
close all

numIter=100;
degThresh=5;

noise = 0.025;
randomness = 0.15;

[X, Y] = meshgrid(1:10, 1:10);
X = X + repmat([0;.5],5,10);
X = [reshape(X, [], 1) reshape(Y, [], 1)] + randomness*randn(100,2);

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
        
        
        
        quadID = getQuad(dt, e(i,:));
        
        quad = X(quadID,:);     
        
%         kyte = [];
%         
%         % for each vertex
%         for v = quadID'
%         
%             % get all the connected vertices that are not in the quad or
%             % are not already processed 
%             [r, ~] = find( e==v );               % get all the edges containing the examined vertex
%             kyte = [ kyte; unique(reshape( e(r,:), [], 1)) ];     % put them in a unique list
%                        
%         end
%         
%         % get rid of the quadID in kyte
%         kyte = setdiff(kyte, quadID);
        
        kyte = getkite(dt, quadID, e);
        
        % now algorithm 2        
        for k = 1:numIter
            
            % add gaussian noise to the quad
            q = quad + noise*randn(4,2);
%             q = quad;
            
            % compute homography H
            H = homography2d(q', uniQuad');
            
            for v = kyte'
               
                % compute the V = H*v                
                V = H*[X(v,:)';1];
                V = V./V(3);
                
                % quantize V
                V = int16(round(V*10));
                % update the LUT
                if ~(isempty(LUT))
                    
                    [ism,b]=ismember(V(1:2)', LUT(:,1:2),'rows' );
                    if(ism)
                        LUT(b,2+i) = LUT(b,2+i)+1;
                    else
                        LUT = [LUT; [V(1) V(2) zeros(1, numEdges)] ];
                        LUT(end,2+i) = LUT(end,2+i)+1;
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
numDEG = 0;

for i = 1:size(e,1)
    
    % get the quad using edgeAttachments
    % t contains the indices of the triangles in dt.Triangulation
    t = edgeAttachments(dt, e(i,1), e(i,2));
   
    
    % if t has 2 triangles
    if ( size(t{1},2) == 2 )
        
               
        quadID = getQuad(dt, e(i,:));
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
        votes = int16(zeros(1, numEdges));
        
        % for each connected neighboor
        for v = kyte'
            V = H*[X(v,:)';1];
            V = V./V(3);
            
            % quantize V
            V = int16(round(V*10));
            
            [ism,b]=ismember(V(1:2)', LUT(:,1:2),'rows' );
            if(ism)
%                 [Y,I] = max(LUT(b,3:end))
                votes = votes + LUT(b,3:end);
            else
                disp('not found');
            end
            
        end
        
        % get the max votes
        [Y,I] = max(votes);
        
        test = degenerateQuad(quad, degThresh);
        
        if (i==I && ~test)
            numOK = numOK+1;
            disp(sprintf('processed edge %03d: \t got index %03d with %d votes', i, I, Y));
        else
            disp(sprintf('processed edge %03d: \t got index %03d with %d votes', i, I, Y));
            [mm, ix] = sort(votes, 'descend');
            disp([mm(1:3)' ix(1:3)'])
            
            if test
                numDEG = numDEG +1;
            else
                numNOK = numNOK+1;
            end
            
%             figure(3)
%             triplot(dt)
%             hold on
% 
%             [are3pointsAligned(quad(1,:)', quad(2,:)', quad(3,:)', degThresh)...
%                 are3pointsAligned(quad(2,:)', quad(3,:)', quad(4,:)', degThresh)...
%                 are3pointsAligned(quad(3,:)', quad(4,:)', quad(1,:)', degThresh)...
%                 are3pointsAligned(quad(4,:)', quad(1,:)', quad(2,:)', degThresh)]
%             plot(X(quadID,1)', X(quadID,2)', '-k')
%             plot(X(quadID,1)', X(quadID,2)', '.m')
%             plot(X(e(i,:),1)', X(e(i,:),2)', '.-r')         
%             plot(X(e(I,:),1)', X(e(I,:),2)', 'o-y')
%             
%             figure(4)
%             plot(uniQuad(:,1)', uniQuad(:,2)', '.-r')
%             V = H*[X(kyte,:)';ones(1,length(kyte))];
%             V = V./repmat(V(3,:),3,1);
%             hold on
%             plot(V(1,:),V(2,:),'.g');
%             wquadID = getQuad(dt, e(I,:));
%             wquad = X(wquadID,:);
%             H = homography2d(wquad', uniQuad');
%             wk = getkite(dt, e(I,:), e);
%             V = H*[X(wk,:)';ones(1,length(wk))];
%             V = V./repmat(V(3,:),3,1);
%             plot(V(1,:),V(2,:),'.b');
%             axis equal
%             pause
%             figure(3)
%             cla
%             figure(4)
%             cla
%         
        end
                
    end
    
end


%%
bb =[1 1; 1 10; 10 10; 10 1]';

bb2 = bb + randn(2,4);

Hgt = homography2d(bb, bb2);

Xgt = Hgt*[X'; ones(1, length(X))];

Xgt = (Xgt(1:2,:)./repmat(Xgt(3,:),2,1))';
% Xgt = [Xgt(:,2) Xgt(:,1)];

dtgt = DelaunayTri(Xgt);

egt = edges(dtgt);
numEdgesegt = size(egt,1);

figure(1)
plot(Xgt(:,1)', Xgt(:,2), '.')

figure(2)
triplot(dtgt)



%% quering  with homography distortion

numOK = 0;
numNOK = 0;
numDEG = 0;

for i = 1:size(egt,1)
    
    % get the quad using edgeAttachments
    % t contains the indices of the triangles in dt.Triangulation
    t = edgeAttachments(dtgt, egt(i,1), egt(i,2));
   
    
    % if t has 2 triangles
    if ( size(t{1},2) == 2 )
               
        quadID = getQuad(dtgt, egt(i,:));
        quad = Xgt(quadID,:);
        
        kyte = getkite(dtgt, quadID, egt);
        
        % now algorithm 1   
        H = homography2d(quad', uniQuad');
        votes = int16(zeros(1, numEdges));
        
        % for each connected neighboor
        for v = kyte'
            V = H*[Xgt(v,:)';1];
            V = V./V(3);
            
            % quantize V
            V = int16(round(V*10));
            
            [ism,b]=ismember(V(1:2)', LUT(:,1:2),'rows' );
            if(ism)
%                 [Y,I] = max(LUT(b,3:end))
                votes = votes + LUT(b,3:end);
            else
                disp('not found');
            end
            
        end
        
        % get the max votes
        [Y,I] = max(votes);
        
        test = degenerateQuad(quad, degThresh);
        
        if ( ~test)
            [mm, ix] = sort(votes, 'descend');
            disp([mm(1:3)' ix(1:3)'])
            
            wquadID = getQuad(dt, e(I,:));
            wquad = X(wquadID,:);
            wk = getkite(dt, e(I,:), e);
            
            [matches, errors] = computeMatches(Xgt(kyte,:), X(wk,:));
            disp(sqrt(mean(errors)))
            
            if sqrt(mean(errors)) < 2.6
                continue
            end
            
            figure(3)
            cla
            triplot(dtgt)
            hold on
            plot(Xgt(quadID,1)', Xgt(quadID,2)', '-k')
            plot(Xgt(quadID,1)', Xgt(quadID,2)', '.m')
            plot(Xgt(egt(i,:),1)', Xgt(egt(i,:),2)', '.-r')         

            
            figure(4)
            cla
            triplot(dt)
            hold on
            quadIDres = getQuad(dt, e(I,:));
            plot(X(quadIDres,1)', X(quadIDres,2)', '-k')
            plot(X(quadIDres,1)', X(quadIDres,2)', '.m')       
            plot(X(e(I,:),1)', X(e(I,:),2)', '.-r')
            
            figure(5)
            cla
            plot(uniQuad(:,1)', uniQuad(:,2)', '.-r')
            V = H*[Xgt(kyte,:)';ones(1,length(kyte))];
            V = V./repmat(V(3,:),3,1);
            hold on
            plot(V(1,:),V(2,:),'.g');
            H1 = homography2d(wquad', uniQuad');
            V = H1*[X(wk,:)';ones(1,length(wk))];
            V = V./repmat(V(3,:),3,1);
            plot(V(1,:),V(2,:),'om');
            axis equal
            
            
            
            figure(6)
            cla
            Hrec = homography2d(quad', wquad');
            V = Hrec*[Xgt';ones(1,length(Xgt))];
            V = V./repmat(V(3,:),3,1);
            plot(V(1,:),V(2,:),'om');
            hold on
            plot(X(:,1)', X(:,2), '.')
            axis equal
            pause

        end
                
    end
    
end
