
Name:       libsvi
Summary:    SVI
Version:    0.1.0
Release:    1.1
Group:      System/Libraries
License:    Apache License v2.0
Source0:    libsvi-%{version}.tar.bz2
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig
BuildRequires:  cmake
BuildRequires:  pkgconfig(dlog)
BuildRequires:  pkgconfig(vconf)
BuildRequires:  pkgconfig(devman_haptic)
BuildRequires:  pkgconfig(mm-keysound)
BuildRequires:  pkgconfig(devman) 


%description
SVI


%package devel
Summary:    SVI
Group:      Development/Libraries
Requires:   %{name} = %{version}-%{release}

%description devel
SVI

%prep
%setup -q 


%build
cmake . -DCMAKE_INSTALL_PREFIX=%{_prefix}


make %{?jobs:-j%jobs}

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

