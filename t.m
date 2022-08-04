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
    r=round((maxr-minr)/stepr)+1;%�������Ĳ�������
    angle=round(2*pi/stepa);
    [m,n]=size(img);
    houghspace=zeros(m,n,r);%����ռ�
    [m1,n1]=find(img);%���ض�ֵ����Ե���ͼ��Img�з��������꣬m1��ź����꣬n1���������
    num=size(m1,1);%��������
    %����ռ䣬ͳ����ͬԲ ��ĸ���
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
    %����ֵ�����Բ
    par=max(max(max(houghspace)));%�ҳ���������Բ��������Ϊ��ֵ
    par2=par*percent;%�ٷֱ�percent��ֵ����
    [m2,n2,r2]=size(houghspace);
    circlefind=[];%�洢������ֵ��Բ��Բ�����꼰�뾶
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