Name:       libsvi
Summary:    SVI
Version:    0.1.0
Release:    30
Group:      System/Libraries
License:    Apache License, Version 2.0
Source0:    %{name}-%{version}.tar.gz
Source1001: packaging/libsvi.manifest 
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
cp %{SOURCE1001} .
cmake . -DCMAKE_INSTALL_PREFIX=%{_prefix}
make

%install
rm -rf %{buildroot}
%make_install

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%manifest libsvi.manifest
%defattr(-,root,root,-)
%{_bindir}/svi_test
%{_libdir}/libsvi.so.*

%files devel
%manifest libsvi.manifest
%defattr(-,root,root,-)
%{_includedir}/svi/*.h
%{_libdir}/libsvi.so
%{_libdir}/pkgconfig/svi.pc

