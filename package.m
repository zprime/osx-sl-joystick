% Script to package the project for uploading the the Mathworks
% FileExchange

function package()

here = pwd();
files = {'src/*.cpp','src/*.hpp','src/*.mexmaci','src/*.mexmaci64','bin/*','README','LICENSE.txt','setup.m'};

try
  cd src;
  % clean up
  [status,result] = system('make cleanest');
  if status
    error('package:CleaningError','Error cleaning up:\n%s\n',result);
  end
  % build binaries
  [status,result] = system('make all');
  if status
    error('package:BuildError','Error compiling the files:\n%s\n',result);
  end
  cd(here);
  
  % Check to see if an archive already exists, if so, delete it
  if exist('osx-sl-joystick.zip','file')
    delete osx-sl-joystick.zip;
  end
  zip('osx-sl-joystick',files);
catch err
  cd(here);
  rethrow(err);
end

fprintf(1,'All done!\n');

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