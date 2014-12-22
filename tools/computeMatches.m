function [ matches, error ] = computeMatches( first, second )
%UNTITLED3 Summary of this function goes here
%   Detailed explanation goes here

error = zeros(size(first,1),1);
matches = zeros(size(first,1),1);

for i = 1:size(first,1)
    
    if isempty(second)
        break
    end

    e = (repmat(first(i,:), size(second,1), 1)-second).^2;
    
    [me, idx] = min(sum(e,2));
    
    error(i) = me;
    
    matches(i) = idx;
    
%     second(idx,:) = [];

end

end

