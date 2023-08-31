#!/bin/bash -l

sacct --format='JobID,JobName,Partition,Elapsed,MaxRSS,MaxVMSize,NNodes,NTasks,State'