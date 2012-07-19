% osx_joystick mask initialization callback helper function
% This function should not be called directly.
function [JoyLocKey,pA,pB,pP,pO,label] = osx_joystick_MaskInitFcn( blk, cbA, cbB, cbP, cbO )
disp('osx_joystick_MaskInitFcn called');

% ASSUMPTION: UserData has been validated by LoadFcn
ud = get_param( blk, 'UserData' );

% If we have selected the empty (NULL) joystick
if ud.SelectedJoystick==0 || isempty(ud.list) || ud.SelectedJoystick > size(ud.list,1)
  JoyLocKey = int32( 0 );
  if cbA; pA=1; else pA=0; end
  if cbB; pB=1; else pB=0; end
  if cbP; pP=1; else pP=0; end
  if cbO; pO=1; else pO=0; end
else
  JoyLocKey = ud.list{ ud.SelectedJoystick, 2 };
  if ud.sizes(1); pA=1; else pA=0; end
  if ud.sizes(2); pB=1; else pB=0; end
  if ud.sizes(3); pP=1; else pP=0; end
  if cbO;         pO=1; else pO=0; end
end

% Update the port labels
label = sprintf('image( imread( ''osx-sl-joystick.png'') );\n');
portnum = 1;
if pA
  if ud.SelectedJoystick~=0; dims=sprintf(' [%i]',ud.sizes(1)); else dims=''; end
  label = [label, sprintf('port_label(''output'',%i,''Axes%s'');\n',portnum,dims)];
  portnum = portnum+1;
end
if pB
  if ud.SelectedJoystick~=0; dims=sprintf(' [%i]',ud.sizes(2)); else dims=''; end
  label = [label, sprintf('port_label(''output'',%i,''Buttons%s'');\n',portnum,dims)];
  portnum = portnum+1;
end
if pP
  if ud.SelectedJoystick~=0; dims=sprintf(' [%i]',ud.sizes(3)); else dims=''; end
  label = [label, sprintf('port_label(''output'',%i,''POVs%s'');\n',portnum,dims)];
end
if pO
  if ud.SelectedJoystick~=0; dims=sprintf(' [%i]',ud.sizes(4)); else dims=''; end
  label = [label, sprintf('port_label(''input'',1,''Outputs%s'');\n',dims)];
end

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