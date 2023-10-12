extension=".singlestep"
for folder in ../tiles/*; do for file in $folder/*.tilewalls1; do 
    echo running on $file
    ./tileblaster $file; done; done
