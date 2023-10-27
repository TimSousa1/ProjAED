for folder in ../tiles/*; do
    for file in $folder/*.tilewalls; do 
        echo running on $file
        time ./src/tileblaster $file; done;
done
