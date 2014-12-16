clear all
close all

numIter=100;

[X, Y] = meshgrid(1:10, 1:10);

X = [reshape(X, [], 1) reshape(Y, [], 1)] + 0.25*randn(100,2);

dt = DelaunayTri(X);


figure(2)
triplot(dt)
hold on
plot(X(:,1)', X(:,2), '.')

for i = 1:100

    [x y] = ginput(1);
    
    X = [X; x, y];
    
    dt = DelaunayTri(X);
    
    cla
    triplot(dt)
    hold on
    plot(X(:,1)', X(:,2), '.')
    
end
