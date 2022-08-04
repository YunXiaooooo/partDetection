clear;
close all;
clc;

gray_image=imread('.\testPic\7.bmp');
% gray_image=rgb2gray(rgb);
% figure,imshow(gray_image);
circlefind = findcircle(gray_image,100,120,2,0.1,1);
% [centersBright,radiiBright,metricBright]=imfindcircles(rgb,[20 25],'ObjectPolarity','bright','Sensitivity',0.92,'EdgeThreshold',0.1);
% figure,imshow(rgb);
% hBright=viscircles(centersBright,radiiBright);
% [centers,radii]=imfindcircles(rgb,[20 25],'ObjectPolarity','dark','Sensitivity',0.92,'Method','twostage');
% h = viscircles(centers,radii);

function[circlefind]=findcircle(img,minr,maxr,stepr,stepa,percent)
    r=round((maxr-minr)/stepr)+1;%可增长的步长个数
    angle=round(2*pi/stepa);
    [m,n]=size(img);
    houghspace=zeros(m,n,r);%霍夫空间
    [m1,n1]=find(img);%返回二值化边缘检测图像Img中非零点的坐标，m1存放横坐标，n1存放纵坐标
    num=size(m1,1);%非零点个数
    %霍夫空间，统计相同圆 点的个数
    %a = x-r*cos(angle), b = y-r*sin(angle)
    for i=1:num
        for j=1:r
            for k=1:angle
                a=round(m1(i)-(minr+(j-1)*stepr)*cos(k*stepa));
                b=round(n1(i)-(minr+(j-1)*stepr)*sin(k*stepa));
                if(a>0&&a<=m&&b>0&&b<=n)
                    houghspace(a,b,j)=houghspace(a,b,j)+1;
                end
            end
        end
    end
    %以阈值来检测圆
    par=max(max(max(houghspace)));%找出个数最多的圆的数量作为阈值
    par2=par*percent;%百分比percent阈值调整
    [m2,n2,r2]=size(houghspace);
    circlefind=[];%存储大于阈值的圆的圆心坐标及半径
    for i=1:m2
        for j=1:n2
            for k=1:r2
                if (houghspace(i,j,k)>=par2)
                    a=[i,j,minr+k*stepr];
                    circlefind=[circlefind;a];
                end
            end
        end
    end
end