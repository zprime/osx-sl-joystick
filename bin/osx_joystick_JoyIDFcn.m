% osx_joystick JoyID parameter callback function
% This function should not be called directly
function osx_joystick_JoyIDFcn( blk )
% ASSUMPTION: UserData has been validated by LoadFcn
ud = get_param( blk, 'UserData' );

% Get the list, and save it
ud.list = osx_joystick_get_available();
str = 'popup(0: None),checkbox,checkbox,checkbox,checkbox,edit';
if ~isempty( ud.list )
  % Escape the evil character | (decimal 124), and replace it with a -
  for ii=1:size(ud.list,1)
    ud.list{ii,1} = regexprep( ud.list{ii,1}, '\|', '-' );
    ud.list{ii,1} = sprintf( '%i: %s', ii, ud.list{ii,1} );
  end
  % Modify the dialogue string
  str = regexprep( str, 'popup\(.*\),checkbox,checkbox,checkbox,checkbox,edit', ['popup(0: None', sprintf([char(124),'%s'],ud.list{:,1}),'),checkbox,checkbox,checkbox,checkbox,edit'], 'once' );
end

vals = get_param( blk, 'MaskValues' );
tmp = regexp( vals{1}, '^(\d+):', 'tokens' );
if ~isempty( tmp )
  ud.SelectedJoystick = str2double( tmp{1} );
else
  ud.SelectedJoystick = 0;
end

% If it's a dummy joystick, show all optoins
if ud.SelectedJoystick == 0
  set_param(blk,'MaskVisibilities',{'on','on','on','on','on','on'});
else
  % If it's a real joystick, hide some of the options and get the sizes.
  set_param(blk,'MaskVisibilities',{'on','off','off','off','on','on'});
  try
    [ud.sizes(1),ud.sizes(2),ud.sizes(3),ud.sizes(4)] = ...
        osx_joystick_get_capabilities( ud.list{ ud.SelectedJoystick, 2 } );
  catch err
    error('osx_joystick:CantGetSizes','Unable to obtain Joystick I/O capabilities. This may happen if you unexpectedly removed the joystick while selecting it at the same time.');
  end
end

% Update the dialogue and UserData
ud.MaskStyleString = str;
set_param(blk,'MaskStyleString',str,'UserData',ud);


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