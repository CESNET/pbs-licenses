#!/usr/bin/env python
import sqlite3
import subprocess
import os

sqlite_db="/var/spool/pbs/license.db"

def get_running_jobs():
    running_jobs=[]

    os.environ["PBSPRO_IGNORE_KERBEROS"] = "1"

    try:
        process = subprocess.Popen("qstat", stdout=subprocess.PIPE)
        output = process.communicate()[0]
    except:
        output = ""

    for line in output.split('\n'):
        splited = line.split()
        if len(splited) < 3:
            continue

        #if splited[-2] != "Q":
        running_jobs.append(splited[0])

    return running_jobs


try:
    conn = sqlite3.connect(sqlite_db)
    c = conn.cursor()

    c.execute("SELECT name FROM sqlite_master WHERE type='table' AND name='licenses'");
    if c.fetchone() == None:
        exit()
except:
    exit()

# ziskam seznam jobu, ktere uz nebezi
running_jobs = get_running_jobs()
jobs_to_remove = []
for row in c.execute('SELECT job_id FROM licenses'):
    if not row[0] in running_jobs:
        jobs_to_remove.append(row[0])

# licence nebezicich jobu odeberu
for job_id in jobs_to_remove:
    c.execute("DELETE FROM licenses WHERE job_id='%s'" % job_id)

conn.commit()
conn.close()
