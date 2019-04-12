executable				= #script
output                	= #logDir/#jobname.out
error                 	= #logDir/#jobname.err
log                   	= #logDir/#jobname.log
requirements			= TARGET.HAS_CMS_HDFS
should_transfer_files   = Yes
when_to_transfer_output = ON_EXIT
#transfer_output_files	= #outfile
transfer_input_files	= #rootmacro, #ccfile1, #hfile, #ccfile2, #filelist
request_memory			= 1000M
request_disk			= 1000M
+JobFlavour 			= "#jobflavour"
queue
