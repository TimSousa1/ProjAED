extension1=".toleblists"
extension2=".tileblasts"
for folder in ../tiles/*; do for file in $folder/*.tilewalls; do 
    echo running on $file
    ./tileblaster $file; done; done
