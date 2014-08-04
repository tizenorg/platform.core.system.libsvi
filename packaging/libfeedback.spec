Name:       libfeedback
Summary:    Feedback library
Version:    0.1.3
Release:    0
Group:      System/Libraries
License:    Apache-2.0
Source0:    %{name}-%{version}.tar.gz
Source1:        libsvi.manifest
Source2:        libfeedback.manifest
Source3:        svi-data.manifest
Requires(post):   /sbin/ldconfig
Requires(postun): /sbin/ldconfig
BuildRequires:  cmake
BuildRequires:  pkgconfig(dlog)
BuildRequires:  pkgconfig(vconf)
BuildRequires:  pkgconfig(mm-keysound)
BuildRequires:  pkgconfig(haptic)
BuildRequires:  pkgconfig(libxml-2.0)
BuildRequires:  pkgconfig(glib-2.0)
BuildRequires:  pkgconfig(capi-base-common)
BuildRequires:  pkgconfig(libtzplatform-config)

%description
Feedback library for playing sound, vibration and led

%package -n libfeedback-devel
Summary:    Feedback library for (devel)
Group:      Development/Libraries
Requires:   libfeedback = %{version}-%{release}

%description -n libfeedback-devel
Feedback library for playing sound, vibration and led (devel)

%package -n libsvi
Summary:    SVI library
Group:      Development/Libraries
Requires:   libfeedback = %{version}-%{release}

%description -n libsvi
SVI library package

%package -n libsvi-devel
Summary:    SVI library for (devel)
Group:      Development/Libraries
Requires:   libsvi = %{version}-%{release}

%description -n libsvi-devel
SVI library (devel) package

%package -n svi-data
Summary:    SVI resource
Group:      Development/Libraries

%description -n svi-data
SVI resource package

%prep
%setup -q 

%build
cp %{SOURCE1} .
cp %{SOURCE2} .
cp %{SOURCE3} .
%cmake .
%__make

%install
rm -rf %{buildroot}
%make_install

mkdir -p %{buildroot}%{_datadir}/license
cp LICENSE.APLv2 %{buildroot}%{_datadir}/license/%{name}

%post -p /sbin/ldconfig

%post -n libsvi -p /sbin/ldconfig

%postun -n libsvi -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%postun -n svi-data
rm -rf %{_datadir}/feedback/

%files
%manifest %{name}.manifest
%defattr(-,root,root,-)
%{_libdir}/libfeedback.so.*
%{_datadir}/license/%{name}

%files -n libfeedback-devel
%manifest %{name}.manifest
%defattr(-,root,root,-)
%{_includedir}/feedback/*.h
%{_libdir}/libfeedback.so
%{_libdir}/pkgconfig/feedback.pc

%files -n libsvi
%manifest %{name}.manifest
%defattr(-,root,root,-)
%{_libdir}/libsvi.so.*

%files -n libsvi-devel
%manifest %{name}.manifest
%defattr(-,root,root,-)
%{_includedir}/svi/*.h
%{_libdir}/libsvi.so
%{_libdir}/pkgconfig/svi.pc

%files -n svi-data
%manifest %{name}.manifest
%defattr(644,root,root,-)
%{_datadir}/feedback/*
