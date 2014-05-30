Shadow_Removal
==============
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
% - SHADOW DETECTION: identification of shadow areas on the image
% - SHADOW REMOVAL: removing shadow areas from the RBG image based on
% the following methods:
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
