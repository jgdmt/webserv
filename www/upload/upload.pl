
use strict;
use warnings;
use CGI;
use CGI::Carp 'fatalsToBrowser';
use File::Basename;

# Directory to save uploads
my $UPLOAD_DIR = $ENV{'IWY_UPLOAD_DIR'};
# print $ENV{'IWY_UPLOAD_DIR'};

# Ensure upload directory exists
mkdir $UPLOAD_DIR unless -d $UPLOAD_DIR;

my $query = CGI->new;

# Print headers
print $query->header('text/html');

# Parse form data
print "<html><body><h1>Upload Results</h1><ul>\r\n";
foreach my $param ($query->param) {
    if ($query->upload($param)) {
        # It's a file upload field
        my $filename = $query->param($param);
        my $basename = basename($filename);
        my $upload_filehandle = $query->upload($param);

        # Save the file
        open my $fh, '>', "$UPLOAD_DIR/$basename" or die "Cannot save file: $!";
        binmode $fh;
        while (my $chunk = <$upload_filehandle>) {
            print $fh $chunk;
        }
        close $fh;

        print "<li>File Field - $param: $basename (uploaded successfully to $UPLOAD_DIR/$basename)</li>\n";
    } else {
        # It's a regular form field
        my $value = $query->param($param);
        print "<li>Field - $param: $value</li>\n";
    }
}
print "</ul></body></html>\r\n";