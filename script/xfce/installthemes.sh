#!/bin/bash

if [ ! -f /etc/centos-release ]; then
	echo "packages are for CentOS only"
	exit
fi

if [ `id -u` != 0 ]; then
	echo "you must be root"
	exit
fi

if [ ! -f /etc/yum.repos.d/nux-dextop.repo ]; then
	echo "must install nux-dextop repository first"
	exit
fi

if [ ! -f /etc/yum.repos.d/epel.repo ]; then
	echo "must install epel repository first"
	exit
fi

if [ -f /etc/yum.repos.d/icons-moka.repo ]; then
	echo "must install icons-moka repository first"
	exit
fi

THEMES="clearlooks-phenix-common clearlooks-phenix-gtk2-theme \
        clearlooks-phenix-xfwm4-theme dmz-cursor-themes \
        xfwm4-themes bluecurve-gtk-themes hicolor-icon-theme \
        mate-icon-theme-faenza zukitwo-gtk2-theme zukitwo-xfwm4-theme \
        moka-icon-theme"

echo "Installing Xfce themes for CentOS"
yum install $THEMES
