% clc;clear;close all;
% 
% Igray= imread("qpg.bmp");
% 
% % Igray = rgb2gray(I);%用已有的函数进行RGB到灰度图像的转换 
% % level = graythresh(Igray);
% % Ibw = im2bw(Igray,level);
% 
% % se = strel('square',5);
% % Igray = imopen(Igray,se);
% % se = strel('square',9);
% % Igray=imerode(Igray,se);
% 
% [imagePoints1,boardSize] = detectCheckerboardPoints(Igray);
% I = insertMarker(Igray, imagePoints1, 'o', 'Color', 'red', 'Size', 50);
% imshow(I)
% imagePoints2 = [];
% baseY = 219;
% basex = 731;
% for i=1:9
%     for j = 9:-1:1
%         imagePoints2 = [imagePoints2;basex+j*161,baseY+i*161];
%     end
% end
% A = [];
% for i=1:81
% A = [A;
%     [0,0,0],                                    -1*[imagePoints1(i,1),imagePoints1(i,2),1],   imagePoints2(i,2)*[imagePoints1(i,1),imagePoints1(i,2),1];
%      1*[imagePoints1(i,1),imagePoints1(i,2),1],  [0,0,0],                                     -imagePoints2(i,1)*[imagePoints1(i,1),imagePoints1(i,2),1];  
%     ];
% end
% [u,s,v] = svd(A);
% 
% H = [v(1,9),v(2,9),v(3,9);
%     v(4,9),v(5,9),v(6,9);
%     v(7,9),v(8,9),v(9,9);
%     ];
% H = 1/H(3,3)*H
% ratio = 100/161

% imagePoints1 = [imagePoints1;1536,1024];
imagePoints1 = H*[imagePoints1 ones(82,1)]';
imagePoints1 = imagePoints1';