Name:       libfeedback
Summary:    Feedback library
Version:    0.1.3
Release:    0
Group:      System/Libraries
License:    Apache License, Version 2.0
Source0:    %{name}-%{version}.tar.gz
Source1:	libsvi.manifest
source2:	libfeedback.manifest
source3:	svi-data-sdk.manifest
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig
BuildRequires:  cmake
BuildRequires:  pkgconfig(dlog)
BuildRequires:  pkgconfig(vconf)
BuildRequires:  pkgconfig(mm-keysound)
BuildRequires:  pkgconfig(haptic)
BuildRequires:  pkgconfig(libxml-2.0)
BuildRequires:  pkgconfig(glib-2.0)
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

%package -n svi-data
Summary: 	svi resource package
Group:		Development/Libraries

%description -n svi-data
svi resource package


%prep
%setup -q 

%build
cp %{SOURCE1} .
cp %{SOURCE2} .
cp %{SOURCE3} .
cmake . -DCMAKE_INSTALL_PREFIX=%{_prefix}
make

%install
rm -rf %{buildroot}
%make_install

mkdir -p %{buildroot}/opt/share/svi/sound/touch
mkdir -p %{buildroot}/opt/share/svi/sound/operation
mkdir -p %{buildroot}/opt/share/svi/haptic/default
mkdir -p %{buildroot}/opt/share/svi/haptic/touch

%post -p /sbin/ldconfig

%post -n svi-data
ln -s %{_datadir}/svi/sound/touch/key0.wav            /opt/share/svi/sound/touch
ln -s %{_datadir}/svi/sound/touch/key1.wav            /opt/share/svi/sound/touch
ln -s %{_datadir}/svi/sound/touch/key2.wav            /opt/share/svi/sound/touch
ln -s %{_datadir}/svi/sound/touch/key3.wav            /opt/share/svi/sound/touch
ln -s %{_datadir}/svi/sound/touch/key4.wav            /opt/share/svi/sound/touch
ln -s %{_datadir}/svi/sound/touch/key5.wav            /opt/share/svi/sound/touch
ln -s %{_datadir}/svi/sound/touch/key6.wav            /opt/share/svi/sound/touch
ln -s %{_datadir}/svi/sound/touch/key7.wav            /opt/share/svi/sound/touch
ln -s %{_datadir}/svi/sound/touch/key8.wav            /opt/share/svi/sound/touch
ln -s %{_datadir}/svi/sound/touch/key9.wav            /opt/share/svi/sound/touch
ln -s %{_datadir}/svi/sound/touch/keyasterisk.wav     /opt/share/svi/sound/touch
ln -s %{_datadir}/svi/sound/touch/keysharp.wav        /opt/share/svi/sound/touch
ln -s %{_datadir}/svi/sound/touch/sip.wav             /opt/share/svi/sound/touch
ln -s %{_datadir}/svi/sound/touch/sip_backspace.wav   /opt/share/svi/sound/touch
ln -s %{_datadir}/svi/sound/touch/touch.wav           /opt/share/svi/sound/touch
ln -s %{_datadir}/svi/sound/operation/call_connect.wav         /opt/share/svi/sound/operation
ln -s %{_datadir}/svi/sound/operation/call_disconnect.wav      /opt/share/svi/sound/operation
ln -s %{_datadir}/svi/sound/operation/charger_connection.wav   /opt/share/svi/sound/operation
ln -s %{_datadir}/svi/sound/operation/fully_charged.wav        /opt/share/svi/sound/operation
ln -s %{_datadir}/svi/sound/operation/list_reorder.wav         /opt/share/svi/sound/operation
ln -s %{_datadir}/svi/sound/operation/lock.wav                 /opt/share/svi/sound/operation
ln -s %{_datadir}/svi/sound/operation/low_battery.wav          /opt/share/svi/sound/operation
ln -s %{_datadir}/svi/sound/operation/minute_minder.wav        /opt/share/svi/sound/operation
ln -s %{_datadir}/svi/sound/operation/power_on.wav             /opt/share/svi/sound/operation
ln -s %{_datadir}/svi/sound/operation/shutter.wav              /opt/share/svi/sound/operation
ln -s %{_datadir}/svi/sound/operation/slider_sweep.wav         /opt/share/svi/sound/operation
ln -s %{_datadir}/svi/sound/operation/unlock.wav               /opt/share/svi/sound/operation
ln -s %{_datadir}/svi/sound/operation/volume_control.wav       /opt/share/svi/sound/operation
ln -s %{_datadir}/svi/haptic/default/Basic_call.tht             /opt/share/svi/haptic/default
ln -s %{_datadir}/svi/haptic/touch/touch.tht                    /opt/share/svi/haptic/touch

%postun -p /sbin/ldconfig

%postun -n svi-data
rm -rf %{_datadir}/svi/
rm -rf /opt/share/svi/

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

%files -n svi-data
%defattr(644,root,root,-)
%{_datadir}/svi/*
%defattr(666,app,app,-)
%dir /opt/share/svi/sound/touch
%dir /opt/share/svi/sound/operation
%dir /opt/share/svi/haptic/default
%dir /opt/share/svi/haptic/touch
%manifest svi-data-sdk.manifest
