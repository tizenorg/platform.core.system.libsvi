Name:       libfeedback
Summary:    Feedback library
Version:    0.1.4
Release:    1
Group:      System/Libraries
License:    Apache-2.0
Source0:    %{name}-%{version}.tar.gz
source1:	libfeedback.manifest
source2:	svi-data.manifest
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig
BuildRequires:  cmake
BuildRequires:  pkgconfig(dlog)
BuildRequires:  pkgconfig(vconf)
BuildRequires:  pkgconfig(mm-keysound)
BuildRequires:  pkgconfig(glib-2.0)
BuildRequires:  pkgconfig(capi-base-common)
BuildRequires:  pkgconfig(dbus-1)
BuildRequires:  pkgconfig(libtzplatform-config)
Requires(post):	svi-data

%description
Feedback library for playing sound and vibration


%package -n libfeedback-devel
Summary:    Feedback library for (devel)
Group:      Development/Libraries
Requires:   libfeedback = %{version}-%{release}

%description -n libfeedback-devel
Feedback library for playing sound and vibration (devel)

%package -n svi-data
Summary: 	svi resource package
Group:		Development/Libraries
Requires:	findutils

%description -n svi-data
svi resource package

%prep
%setup -q

%build
export CFLAGS+=" -DTIZEN_ENGINEER_MODE"
cp %{SOURCE1} .
cp %{SOURCE2} .

%cmake . \
		-DCMAKE_INSTALL_PREFIX=%{_prefix} \
		-DPROFILE=%{profile} \
		-DTZ_SYS_RO_SHARE=%{TZ_SYS_RO_SHARE} \
		-DTZ_SYS_SHARE=%{TZ_SYS_SHARE}
make

%install
rm -rf %{buildroot}
%make_install

mkdir -p %{buildroot}%{_datadir}/license
cp LICENSE %{buildroot}%{_datadir}/license/%{name}
cp LICENSE %{buildroot}%{_datadir}/license/svi-data

mkdir -p %{buildroot}%{TZ_SYS_SHARE}/feedback/haptic/custom

%post -p /sbin/ldconfig

%post -n svi-data

%postun -p /sbin/ldconfig

%postun -n svi-data
rm -rf %{_datadir}/feedback/
rm -rf %{TZ_SYS_SHARE}/feedback/

%files
%defattr(-,root,root,-)
%{_libdir}/libfeedback.so.*
%{_datadir}/license/%{name}
%manifest libfeedback.manifest

%files -n libfeedback-devel
%defattr(-,root,root,-)
%{_includedir}/feedback/*.h
%{_libdir}/libfeedback.so
%{_libdir}/pkgconfig/feedback.pc

%files -n svi-data
%defattr(644,root,root,-)
%{_datadir}/feedback/*
%defattr(666,app,app,-)
%dir %{TZ_SYS_SHARE}/feedback/haptic/custom
%{_datadir}/license/svi-data
%manifest svi-data.manifest
