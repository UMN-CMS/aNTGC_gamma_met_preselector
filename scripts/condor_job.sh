executable				= #script
output                	= #logDir/#jobname.out
error                 	= #logDir/#jobname.err
log                   	= #logDir/#jobname.log
requirements			= TARGET.HAS_CMS_HDFS
transfer_input_files	= #rootmacro, #ccfile1, #hfile, #ccfile2, #filelist
transfer_output_files	= #outfile
output_destination		= #outdir
request_memory			= 1000M
request_disk			= 1000M
+JobFlavour 			= "#jobflavour"
queue
