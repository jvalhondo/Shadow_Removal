%*************************************************************************%
% file: Shadow_Removal_VA_jvr.m
% 
% Author: Jorge Valhondo Rama
% 
% License: GNU General Public License v3. Feel free to share and change it!
%
% 
% SHADOW REMOVAL from RBG images. It is focused on removing shadow areas from
% the image and not on the detection. The script consists of two modules:
%
%   - SHADOW DETECTION: identification of shadow areas on the image
%   - SHADOW REMOVAL: removing shadow areas from the RBG image based on
%                     the following methods: 
%
% ------------------------- SHADOW REMOVAL METHODS ------------------------ 
% 1. ADDITIVE:
% Adjusting shadow & light pixel intensities by adding the difference of pixel
% averages to the shadow pixels.
%
% 2. BASIC LIGHT MODEL:
% Basic light model containing an ambient & a directed light ratio.
%
% 3. ENHANCED LIGHT MODEL:
% Enhanced shadow removal model containing an ambient & a directed light ratio
% and shadow coefficient.
%
% 4. im_ycbcr COLORSPACE:
% Hybridation of the additive and light model based methods on im_ycbcr colourspace.
%
%*************************************************************************%

clear all, close all, clc
% IMAGE: read and show RGB image
image = double(imread('images/.png'))./255; % colour values between 0 and 1
s_im = size(image);

%*************************************************************************%

% SHADOW DETECTION
    % MASK: creating a shadow segmentation if no mask is available
    gray = rgb2gray(image);
    light_mask = double(bwareaopen(im2bw(gray, graythresh(gray)),200));
    h = fspecial('gaussian',20,0.5);
    light_mask = imfilter(light_mask,h);
    shadow_mask = 1 - light_mask;
    

    % SHADOW / LIGHT CORE DETECTION
    % structuring element
    struct_elem = [0 1 1 1 0; 1 1 1 1 1; 1 1 1 1 1; 1 1 1 1 1; 0 1 1 1 0];
    
    % shadow/light  core (morphology erode: pixels not on the blurred edge of the shadow area)
    shadow_core = imerode(shadow_mask, struct_elem);
    light_core = imerode(light_mask, struct_elem);
    % smoothing the mask
    smoothmask = conv2(shadow_mask, struct_elem/sum(sum(struct_elem)), 'same');
    
    % AVERAGE PIXEL INTENSITIES
    % shadow area
    shadow_avg_red = sum(sum(image(:,:,1).*shadow_core)) / sum(sum(shadow_core));
    shadow_avg_green = sum(sum(image(:,:,2).*shadow_core)) / sum(sum(shadow_core));
    shadow_avg_blue = sum(sum(image(:,:,3).*shadow_core)) / sum(sum(shadow_core));
    % light area
    light_avg_red = sum(sum(image(:,:,1).*light_core)) / sum(sum(light_core));
    light_avg_green = sum(sum(image(:,:,2).*light_core)) / sum(sum(light_core));
    light_avg_blue = sum(sum(image(:,:,3).*light_core)) / sum(sum(light_core));
    
%*************************************************************************%

% SHADOW REMOVAL: different methods
    
    % Method 1: ADDITIVE SHADOW REMOVAL
    result_additive = zeros(s_im);
    % compiting colour difference between the shadow/lit areas
    diff_red = light_avg_red - shadow_avg_red;
    diff_green = light_avg_green - shadow_avg_green;
    diff_blue = light_avg_blue - shadow_avg_blue;
    % adding the difference to the shadow pixels
    result_additive(:,:,1) = image(:,:,1) + smoothmask * diff_red;
    result_additive(:,:,2) = image(:,:,2) + smoothmask * diff_green;
    result_additive(:,:,3) = image(:,:,3) + smoothmask * diff_blue;
    
    %---------------------------------------------------------------------%
    
    % Method 2: BASIC , LIGHT MODEL BASED SHADOW REMOVAL
    result_basic_model = zeros(s_im);
    % computing ratio of shadow/lit area luminance
    ratio_red = light_avg_red/shadow_avg_red;
    ratio_green = light_avg_green/shadow_avg_green;
    ratio_blue = light_avg_blue/shadow_avg_blue;
    % multiplying the shadow pixels with the raio for the correction
    result_basic_model(:,:,1) = image(:,:,1).*light_mask + shadow_mask.*ratio_red.*image(:,:,1);
    result_basic_model(:,:,2) = image(:,:,2).*light_mask + shadow_mask.*ratio_green.*image(:,:,2);
    result_basic_model(:,:,3) = image(:,:,3).*light_mask + shadow_mask.*ratio_blue.*image(:,:,3);
    
    %---------------------------------------------------------------------%
    
    % Method 3: ADVANCE, LIGHT MODEL BASED SHADOW REMOVAL
    result_enhanced_model = zeros(s_im);
    % computing ratio of the luminances of the directed, and global lights
    ratio_red = light_avg_red/shadow_avg_red - 1;
    ratio_green = light_avg_green/shadow_avg_green - 1;
    ratio_blue = light_avg_blue/shadow_avg_blue - 1;
    % applying shadow removal formula
    result_enhanced_model(:,:,1) = (ratio_red + 1)./((1-smoothmask)*ratio_red + 1).*image(:,:,1);
    result_enhanced_model(:,:,2) = (ratio_green + 1)./((1-smoothmask)*ratio_green + 1).*image(:,:,2);
    result_enhanced_model(:,:,3) = (ratio_blue + 1)./((1-smoothmask)*ratio_blue + 1).*image(:,:,3);
   
    %---------------------------------------------------------------------%
    
    % Method 4: COMBINED ADDITIVE AND LIGHT MODEL BASED SHADOW REMOVAL IN im_ycbcr COLOURSPACE
    % conversion to YCbCr colorspace: Y->luminance, Cb->blue-difference, Cr->red-difference
    im_ycbcr = rgb2ycbcr(image);
    % computing average channel values in YCbCr colorspace
    shadow_avg_y = sum(sum(im_ycbcr(:,:,1).*shadow_core)) / sum(sum(shadow_core));
    shadow_avg_cb = sum(sum(im_ycbcr(:,:,2).*shadow_core)) / sum(sum(shadow_core));
    shadow_avg_cr = sum(sum(im_ycbcr(:,:,3).*shadow_core)) / sum(sum(shadow_core));
    %
    light_avg_y = sum(sum(im_ycbcr(:,:,1).*light_core)) / sum(sum(light_core));
    light_avg_cb = sum(sum(im_ycbcr(:,:,2).*light_core)) / sum(sum(light_core));
    light_avg_cr = sum(sum(im_ycbcr(:,:,3).*light_core)) / sum(sum(light_core));
    % computing ratio, and difference in im_ycbcr space
    ratio_y = light_avg_y/shadow_avg_y;
    ratio_cb = light_avg_cb/shadow_avg_cb;
    ratio_cr = light_avg_cr/shadow_avg_cr;
    %
    diff_y = light_avg_y - shadow_avg_y;
    diff_cb = light_avg_cb - shadow_avg_cb;
    diff_cr = light_avg_cr - shadow_avg_cr;
    % shadow correction: Y->additive correction, Cb&Cr-> basic light model correction
    aux_result_im_ycbcr = im_ycbcr;
    aux_result_im_ycbcr(:,:,1) = im_ycbcr(:,:,1) + shadow_mask*diff_y;
    aux_result_im_ycbcr(:,:,2) = im_ycbcr(:,:,2).*light_mask + shadow_mask.*ratio_cb.*im_ycbcr(:,:,2);
    aux_result_im_ycbcr(:,:,3) = im_ycbcr(:,:,3).*light_mask + shadow_mask.*ratio_cr.*im_ycbcr(:,:,3);
    % back to RGB colourspace
    result_im_ycbcr = ycbcr2rgb(aux_result_im_ycbcr);
 
%*************************************************************************%

% SHOW RESULTS:
    
    % Show original image
    figure, imshow(image), title('Original Image')
    
    % Show Masks: Light, Shadow, Smooth
    figure,
    subplot(1,3,1), imshow(light_mask), title('Light Mask')
    subplot(1,3,2), imshow(shadow_mask), title('Shadow Mask')
    subplot(1,3,3), imshow(smoothmask), title('Smooth Mask')
    
    % Show result ADDITIVE, BASIC LIGHT MODEL, ENHANCED LIGHT MODEL, YCbCr methods
    figure, 
    subplot(2,2,1), imshow(result_additive), title('Shadow Removal: Additive method')
    subplot(2,2,2), imshow(result_basic_model), title('Shadow Removal: Basic light model method')
    subplot(2,2,3), imshow(result_enhanced_model), title('Shadow Removal: Enhanced light model method')
    subplot(2,2,4), imshow(result_im_ycbcr), title('Shadow Removal: YC_bC_r method')
    