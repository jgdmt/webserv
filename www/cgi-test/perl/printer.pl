use strict;
use warnings;
use CGI;

# Create a new CGI object
my $cgi = CGI->new;

# Read the body content
my $body = '';
if ($ENV{'CONTENT_LENGTH'}) {
    read(STDIN, $body, $ENV{'CONTENT_LENGTH'});
}


print $cgi->header('text/html');
print $cgi->start_html(
    -title => 'Print Result',
    -style => {-src => 'main.css'}  );

print $cgi->div({-id => 'content'},
        $cgi->h1('Print:'),
        $cgi->p($body));
print $cgi->end_html;

exit 0;