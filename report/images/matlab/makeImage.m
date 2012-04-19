close all;
clear all
dva=imread('2.png');
dva_=dva(365:800,1152:1662,:);

tri=imread('3.png');
tri_=tri(365:800,1152:1662,:);

stiri=imread('4.png');
stiri_=stiri(365:800,1152:1662,:);

pet=imread('5.png');
pet_=pet(365:800,1152:1662,:);

image(pet_)

slika=dva_;
slika(200:end,340:end,:)=tri_(200:end,340:end,:);
slika(1:65,230:280,:)=stiri_(1:65,230:280,:);
slika(260:315,450:490,:)=pet_(260:315,450:490,:);
image(slika)
imsave