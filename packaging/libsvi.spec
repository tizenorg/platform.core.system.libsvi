Name:       libsvi
Summary:    SVI
Version:    0.1.2
Release:    7
Group:      System/Libraries
License:    Apache License, Version 2.0
Source0:    %{name}-%{version}.tar.gz
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig
BuildRequires:  cmake
BuildRequires:  pkgconfig(dlog)
BuildRequires:  pkgconfig(vconf)
BuildRequires:  pkgconfig(devman_haptic)
BuildRequires:  pkgconfig(mm-keysound)
BuildRequires:  pkgconfig(devman)

%description

%package devel
Summary:    SVI
Group:      Development/Libraries
Requires:   %{name} = %{version}-%{release}

%description devel

%prep
%setup -q 

%build
cmake . -DCMAKE_INSTALL_PREFIX=%{_prefix}
make

%install
rm -rf %{buildroot}
%make_install

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(-,root,root,-)
%{_bindir}/svi_test
%{_libdir}/libsvi.so.*

%files devel
%defattr(-,root,root,-)
%{_includedir}/svi/*.h
%{_libdir}/libsvi.so
%{_libdir}/pkgconfig/svi.pc

%changelog
* Mon Aug 20 2012 - Jiyoung Yun <jy910.yun@samsung.com>
- Re-uploaded because prev commit is not supply on tizen_build project
- Tag : libsvi_0.1.2-7

* Fri Aug 17 2012 - Jiyoung Yun <jy910.yun@samsung.com>
- separate 2.0_beta branch from master
- Remove code regarding led notification that is product feature
- Tag : libsvi_0.1.2-6

* Tue Aug 14 2012 - Jiyoung Yun <jy910.yun@samsung.com>
- led bug fix
- svi_play_led function is called twice
- Tag : libsvi_0.1.2-5

* Tue Aug 14 2012 - Jiyoung Yun <jy910.yun@samsung.com>
- Code merge from old git
- Tag : libsvi_0.1.2-4

* Mon Aug 06 2012 - Jiyoung Yun <jy910.yun@samsung.com>
- Add new code for support LED notification
- Add svi-led.c file to convert ivt to led file
- Change SVI_VIB_OPERATION_FULLYCHARGED to SVI_VIB_OPERATION_FULLCHARGED enum value
- Tag : libsvi_0.1.2-3

* Fri Aug 03 2012 - Jiyoung Yun <jy910.yun@samsung.com>
- Change SVI_VIB_OPERATION_LOTBATT to SVI_VIB_OPERATION_LOWBATT value
- Tag : libsvi_0.1.2-2

* Fri Jul 13 2012 - Hyungdeuk Kim <hd3.kim@samsung.com>
- Add 2 new enum type defined by ux 0627 document
- remove temp code for error handling
