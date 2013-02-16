Name:       libfeedback
Summary:    Feedback library
Version:    0.1.2
Release:    21
Group:      System/Libraries
License:    Apache License, Version 2.0
Source0:    %{name}-%{version}.tar.gz
Source1:	libsvi.manifest
source2:	libfeedback.manifest
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig
BuildRequires:  cmake
BuildRequires:  pkgconfig(dlog)
BuildRequires:  pkgconfig(vconf)
BuildRequires:  pkgconfig(mm-keysound)
BuildRequires:  pkgconfig(haptic)
BuildRequires:  pkgconfig(capi-base-common)

%description
Feedback library for playing sound, vibration and led


%package -n libfeedback-devel
Summary:    Feedback library for (devel)
Group:      Development/Libraries
Requires:   libfeedback = %{version}-%{release}

%description -n libfeedback-devel
Feedback library for playing sound, vibration and led (devel)

%package -n libsvi
Summary:	SVI library
Group:		Development/Libraries
Requires:	libfeedback = %{version}-%{release}

%description -n libsvi
SVI library

%package -n libsvi-devel
Summary:	SVI library for (devel)
Group:		Development/Libraries
Requires:	libsvi = %{version}-%{release}

%description -n libsvi-devel
SVI library (devel)


%prep
%setup -q 

%build
cp %{SOURCE1} .
cp %{SOURCE2} .
cmake . -DCMAKE_INSTALL_PREFIX=%{_prefix}
make

%install
rm -rf %{buildroot}
%make_install

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%manifest libfeedback.manifest
%defattr(-,root,root,-)
%{_libdir}/libfeedback.so.*

%files -n libfeedback-devel
%defattr(-,root,root,-)
%{_includedir}/feedback/*.h
%{_libdir}/libfeedback.so
%{_libdir}/pkgconfig/feedback.pc

%files -n libsvi
%manifest libsvi.manifest
%defattr(-,root,root,-)
%{_libdir}/libsvi.so.*

%files -n libsvi-devel
%defattr(-,root,root-)
%{_includedir}/svi/*.h
%{_libdir}/libsvi.so
%{_libdir}/pkgconfig/svi.pc

%changelog
* Mon Dec 10 2012 - Jae-young Hwang <j-zero.hwang@samsung.com>
- Revise _feedback_play_vibration function for playing tht files.
- Tag : libfeedback_0.1.2-17

* Wed Nov 21 2012 - Jiyoung Yun <jy910.yun@samsung.com>
- add FEEDBACK_PATTERN_MAX_CHARACTER enum
- Tag : libfeedback_0.1.2-16
