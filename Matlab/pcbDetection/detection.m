function pcd()

%对待检测PCB板预处理
function tgtImg = pre_process(srcImg)

srcImg_gray = rgb2gray(srcImg);%灰度化
src_med = medfilt2(srcImg_gray);%中值滤波
src_bw = im2bw(src_med);%二值化

src_spur = bwmorph(src_bw,'spur',8);%去除物体小的分支
src_fill = bwmorph(src_spur,'fill');%填充孤立黑点
tgtImg = bwmorph(src_fill,'clean');%去除孤立亮点
end

%对标准PCB板图像进行预处理
%认为标准PCB板的图像为理想情况，只用进行灰度化和二值化
function tgtImg = getBW(srcImg)

srcImg_gray = rgb2gray(srcImg);%灰度化
srcImg_bw = im2bw(srcImg_gray);%二值化
tgtImg = srcImg_bw;

end
%对待测PCB图像进行旋转
function tgtImg = roateImg(stdImg,srcImg)
    
src_rect=[80 370 150 130];%待检测pcb图像中选取参与互相关计算区域的矩阵
std_rect=[40 320 200 190];% 标准pcb图像中选取参与互相关计算区域的矩阵
std_sub=imcrop(stdImg,std_rect);%剪裁标准pcb图像
max_c=0;%初始化互相关最大值
    for rr=-2:1:2%待检测pcb图像依次旋转的角度（步进值可调）
        src_rot=imrotate(srcImg,rr,'nearest');%待检测pcb图像旋转，使用邻近插值法
        src_sub=imcrop(src_rot,src_rect);%裁剪带检测pcb图像
        c=normxcorr2(src_sub,std_sub);%计算互相系数
        [max_c1,imax1]=max(abs(c(:)));%max_c1为系数最大值，imax1为系数最大值对应的位置下标
        if(max_c1>max_c)%每一次循环的最大值进行比较
            max_c=max_c1;%取最大的值
            angle=rr;%把取得最大值时对应的旋转角度赋给angle
        end
    end
src_tz=imrotate(srcImg,angle,'nearest');%按angle角，对待检测pcb图像进行旋转修正
src_tz_sub=imcrop(src_tz,src_rect);%此时两幅图像的轴向已平行，重新计算互相关系数
cc=normxcorr2(src_tz_sub,std_sub);%
[max_cc,imax]=max(abs(cc(:)));%
[ypeak,xpeak]=ind2sub(size(cc),imax);%将下标转化为行列的表示形式  
yd=[ypeak-(src_rect(4)+1) xpeak-(src_rect(3)+1)];%子图需移动的量
std_dc=[yd(1)+std_rect(2) yd(2)+std_rect(1)];%标准pcb图像在调整后的待检测图像中的坐标
xz=[std_dc(1)-src_rect(2) std_dc(2)-src_rect(1)];%像素修正值
dc_qu_rect=[1-xz(2) 1-xz(1) size(stdImg,2)-1 size(stdImg,1)-1]; %调整后的待检测图像中选取与标准图像同等大小的区域矩阵
dc_qu=imcrop(src_tz,dc_qu_rect);%裁剪调整后的待检测pcb图像
tgtImg = dc_qu;
  
end
function tgtImg = xorImg(stdImg,srcImg,std_rgb)
 ImgXor=xor(stdImg,srcImg);%图像异或运算
 MN=[3 3]; 
 se=strel('rectangle',MN);%定义结构元素
 imr=imerode(ImgXor,se);%腐蚀运算
 imd=imdilate(imr,se);%膨胀运算
 rgb=label2rgb(imd,@autumn,'g');%标注对象变为彩色，采用autumn映射表，背景为绿色
 tgtImg=imlincomb(.6,rgb,.4,std_rgb);%将两幅图像按比例线性组合

   
end


std_rgb =  imread('standard.jpg');
figure()
imshow(std_rgb);
title('标准图');

stdImg = getBW(std_rgb);
figure()
imshow(stdImg);
title('标准图二值化');

src_rgb = imread('untreated.jpg');
figure()
imshow(src_rgb);
title('待检测图');

srcImg = pre_process(src_rgb);
figure()
imshow(srcImg);
title('待检测图二值化');

srcRot = roateImg(stdImg,srcImg);
figure();
imshow(srcRot);
title('匹配后的待检测pcb图像');

markedImg = xorImg(stdImg,srcRot,std_rgb);
figure();
imshow(markedImg);
title('缺陷标注');  
end
