%define p_path /var/spool/pbs

Name: pbs-licences		
Version: 1.0.0
Release: 1%{?dist}
Summary: PBS Licences

Group: System/Base	
License: None
URL: cerit-sc.cz
Source0: %{name}-%{version}.tar.gz	

BuildRoot: %{buildroot}
BuildRequires: gcc
BuildRequires: libpbs-cache
BuildRequires: sqlite
Requires: libpbs-cache
Requires: sqlite

%description
PBS Licences is software for managing licences for PBS jobs.

%prep
%setup -q


%build
#%{__make} %{?_smp_mflags}
cd pbs-licences/var/spool/pbs
gcc get_license.c -o get_license -lpbscache -l sqlite3


%install
#make install DESTDIR=%{buildroot}
rm -rf %{buildroot}
mkdir -p %{buildroot}/var/spool/pbs
mkdir -p %{buildroot}/var/spool/pbs/licenses
mkdir -p %{buildroot}/etc/cron.d
mkdir -p %{buildroot}/etc/default
cd pbs-licences
cp -a ./* %{buildroot}/

%files
%defattr(-,root,root)
%config /etc/default/pbs_licenses
/etc/cron.d/pbs_licences
%dir %{p_path}
%{p_path}/*
%exclude /var/spool/pbs/get_license.c
%exclude /var/spool/pbs/*.pyc
%exclude /var/spool/pbs/*.pyo
