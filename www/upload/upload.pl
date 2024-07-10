use strict;
use warnings;
use CGI;
use CGI::Carp 'fatalsToBrowser';
use File::Basename;


my $UPLOAD_DIR = $ENV{'IWY_UPLOAD_DIR'};

my $cgi = CGI->new;

# Print headers
print $cgi->header('text/html');
print $cgi->start_html(
    -title => 'Upload Result',
    -style => {-src => 'main.css'}
);

# Parse form data
print $cgi->start_div({-id => 'content'}, '');
if ($UPLOAD_DIR eq '')
{
    print $cgi->h1('No upload directory set');
}
else {
    print $cgi->h1('Uploaded Files:');
    print $cgi->start_ul;
    foreach my $param ($cgi->param) {
        if ($cgi->upload($param)) {
            # It's a file upload field
            my $filename = $cgi->param($param);
            my $basename = basename($filename);
            my $upload_filehandle = $cgi->upload($param);

            # Save the file
            open my $fh, '>', "$UPLOAD_DIR/$basename" or do {
                print $cgi->li("$basename: Cannot save file.");
                next;
            };
            binmode $fh;
            while (my $chunk = <$upload_filehandle>) {
                print $fh $chunk;
            }
            close $fh;

            print $cgi->li("$basename: uploaded successfully.");
        } else {
            # It's a regular form field
            my $value = $cgi->param($param);
            print $cgi->li("$param: $value isn't a file");
        }
    }
    print $cgi->end_ul;
}
print $cgi->end_div,
print $cgi->end_html;

exit 0;