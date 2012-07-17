%SETUP Sets up osx-sl-joystick for use in Simulink
% setup( DESTDIR )
%
% This function compiles (if necessary) the binaries and adds files to the
% MATLAB path. The Mathworks FileExchange no longer allows the distribution
% of binaries, so you will have to compile this project yourself. Help can
% be found on the GitHub development site <a href="https://github.com/zprime/osx-sl/joystick/wiki">wiki</a>.
%
% Input:
%  DESTDIR - Optional.  If specified, the files are installed to the
%    specified directory.
%
% Example:
%  setup( '~/Documents/MATLAB/Library/osx-sl-joystick' );
%
% Note that if the files require XCode to be installed. For recent versions
% of Matlab, you may need to apply <a
% href="http://www.mathworks.com/support/solutions/en/data/1-FR6LXJ/">this patch</a> to enable the XCode g++
% compiler work with mex.
%
% Please post bug reports on the <a href="https://github.com/zprime/osx-sl-joystick/issues">GitHub issues</a> project page.
function setup( destdir )

% Sanity check the input
if nargin > 1
  error('setup:tooManyParams','setup.m accepts a maximum of one parameter (destination directory).');
end
if nargin == 1
  if ~isa( destdir, 'char' )
    error('setup:DestMustBeString','Destination directory must be specified as a string.');
  end
end

% List of mex functions that need to be compiled
mexnames = {'osx_joystick_get_available','osx_joystick_get_capabilities','sfun_osx_joystick'};
% Other files that need to be linked to the mex files
libnames = {'osx_joystick.cpp','axes.cpp','button.cpp','pov.cpp','outputs.cpp'};

% Loop through and compile the files if needed. Then copy them to the
% ../bin/ directory if needed.
here = pwd;
cd src
try
  for ii=1:length(mexnames)
    if ~exist( [mexnames{ii},'.',mexext()], 'file' )
      mex('-v','LDFLAGS=\$LDFLAGS -framework IOKit -framework CoreFoundation',...
        [mexnames{ii},'.cpp'],libnames{:});
      copyfile( [mexnames{ii},'.',mexext()], '../bin/', 'f' );
    elseif ~exist( ['../bin/',mexnames{ii},'.',mexext()], 'file' )
      copyfile( [mexnames{ii},'.',mexext()], '../bin/', 'f' );
    end
  end
  cd(here);
catch err
  cd(here);
  rethrow(err);
end

% If a destination directory was specified, make and copy the files to it.
if nargin == 1
  [s,m,mid] = mkdir( destdir );
  if ~s
    error(mid,'Error making destination directory:\n%s',m);
  end
  [s,m,mid] = copyfile( 'bin/*', destdir, 'f' );
  if ~s
    error(mid,'Error copying files to destination directory:\n%s',m);
  end
else
  destdir = [pwd(),'/bin'];
end
 
% Now add the directory to the path
addpath( destdir, '-end' );
if savepath
  error('setup:CantSavePath','Error saving the Matlab path.');
end

% Done!
fprintf( 1, ['OS X Simulink Joystick has now been successfully installed to:\n',...
    '%s\nIf the Simulink Library Browser has already been started, you may\n'...
    'need to restart Matlab before the block will appear.\n',...
    'The block should appear in the library as ''OS X Joystick''.\n',...
    'To uninstall, remove the destination from the path and delete the directory:\n',...
    '  >> rmpath %s\n',...
    '  >> savepath\n',...
    '  >> rmdir %s s\n'],...
    destdir, destdir, destdir );

% Copyright (c) 2012, Zebb Prime and The University of Adelaide
% All rights reserved.
%  
% Redistribution and use in source and binary forms, with or without
% modification, are permitted provided that the following conditions are met:
%     * Redistributions of source code must retain the above copyright
%       notice, this list of conditions and the following disclaimer.
%     * Redistributions in binary form must reproduce the above copyright
%       notice, this list of conditions and the following disclaimer in the
%       documentation and/or other materials provided with the distribution.
%     * Neither the name of the organization nor the
%       names of its contributors may be used to endorse or promote products
%       derived from this software without specific prior written permission.
% 
% THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
% ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
% WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
% DISCLAIMED. IN NO EVENT SHALL ZEBB PRIME OR THE UNIVERSITY OF ADELAIDE BE LIABLE FOR ANY
% DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
% (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
% LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
% ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
% (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
% SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.