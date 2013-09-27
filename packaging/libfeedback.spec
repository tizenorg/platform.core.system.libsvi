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
%cmake .
make

%install
rm -rf %{buildroot}
%make_install

mkdir -p %{buildroot}/opt/usr/share/feedback/sound/touch
mkdir -p %{buildroot}/opt/usr/share/feedback/sound/operation
mkdir -p %{buildroot}/opt/usr/share/feedback/haptic/default
mkdir -p %{buildroot}/opt/usr/share/feedback/haptic/touch

%post -p /sbin/ldconfig

%post -n libsvi -p /sbin/ldconfig

%postun -n libsvi -p /sbin/ldconfig

%post -n svi-data
ln -s %{_datadir}/feedback/sound/touch/key0.wav            /opt/usr/share/feedback/sound/touch
ln -s %{_datadir}/feedback/sound/touch/key1.wav            /opt/usr/share/feedback/sound/touch
ln -s %{_datadir}/feedback/sound/touch/key2.wav            /opt/usr/share/feedback/sound/touch
ln -s %{_datadir}/feedback/sound/touch/key3.wav            /opt/usr/share/feedback/sound/touch
ln -s %{_datadir}/feedback/sound/touch/key4.wav            /opt/usr/share/feedback/sound/touch
ln -s %{_datadir}/feedback/sound/touch/key5.wav            /opt/usr/share/feedback/sound/touch
ln -s %{_datadir}/feedback/sound/touch/key6.wav            /opt/usr/share/feedback/sound/touch
ln -s %{_datadir}/feedback/sound/touch/key7.wav            /opt/usr/share/feedback/sound/touch
ln -s %{_datadir}/feedback/sound/touch/key8.wav            /opt/usr/share/feedback/sound/touch
ln -s %{_datadir}/feedback/sound/touch/key9.wav            /opt/usr/share/feedback/sound/touch
ln -s %{_datadir}/feedback/sound/touch/keyasterisk.wav     /opt/usr/share/feedback/sound/touch
ln -s %{_datadir}/feedback/sound/touch/keysharp.wav        /opt/usr/share/feedback/sound/touch
ln -s %{_datadir}/feedback/sound/touch/sip.wav             /opt/usr/share/feedback/sound/touch
ln -s %{_datadir}/feedback/sound/touch/sip_backspace.wav   /opt/usr/share/feedback/sound/touch
ln -s %{_datadir}/feedback/sound/touch/touch.wav           /opt/usr/share/feedback/sound/touch
ln -s %{_datadir}/feedback/sound/operation/call_connect.wav         /opt/usr/share/feedback/sound/operation
ln -s %{_datadir}/feedback/sound/operation/call_disconnect.wav      /opt/usr/share/feedback/sound/operation
ln -s %{_datadir}/feedback/sound/operation/charger_connection.wav   /opt/usr/share/feedback/sound/operation
ln -s %{_datadir}/feedback/sound/operation/fully_charged.wav        /opt/usr/share/feedback/sound/operation
ln -s %{_datadir}/feedback/sound/operation/list_reorder.wav         /opt/usr/share/feedback/sound/operation
ln -s %{_datadir}/feedback/sound/operation/lock.wav                 /opt/usr/share/feedback/sound/operation
ln -s %{_datadir}/feedback/sound/operation/low_battery.wav          /opt/usr/share/feedback/sound/operation
ln -s %{_datadir}/feedback/sound/operation/minute_minder.wav        /opt/usr/share/feedback/sound/operation
ln -s %{_datadir}/feedback/sound/operation/power_on.wav             /opt/usr/share/feedback/sound/operation
ln -s %{_datadir}/feedback/sound/operation/shutter.wav              /opt/usr/share/feedback/sound/operation
ln -s %{_datadir}/feedback/sound/operation/slider_sweep.wav         /opt/usr/share/feedback/sound/operation
ln -s %{_datadir}/feedback/sound/operation/unlock.wav               /opt/usr/share/feedback/sound/operation
ln -s %{_datadir}/feedback/sound/operation/volume_control.wav       /opt/usr/share/feedback/sound/operation
ln -s %{_datadir}/feedback/haptic/default/Basic_call.tht             /opt/usr/share/feedback/haptic/default
ln -s %{_datadir}/feedback/haptic/touch/touch.tht                    /opt/usr/share/feedback/haptic/touch

%postun -p /sbin/ldconfig

%postun -n svi-data
rm -rf %{_datadir}/feedback/
rm -rf /opt/usr/share/feedback/

%files
%manifest %{name}.manifest
%defattr(-,root,root,-)
%{_libdir}/libfeedback.so.*

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
%defattr(-,root,root-)
%{_includedir}/svi/*.h
%{_libdir}/libsvi.so
%{_libdir}/pkgconfig/svi.pc

%files -n svi-data
%manifest %{name}.manifest
%defattr(644,root,root,-)
%{_datadir}/feedback/*
%defattr(666,app,app,-)
%dir /opt/usr/share/feedback/sound/touch
%dir /opt/usr/share/feedback/sound/operation
%dir /opt/usr/share/feedback/haptic/default
%dir /opt/usr/share/feedback/haptic/touch
