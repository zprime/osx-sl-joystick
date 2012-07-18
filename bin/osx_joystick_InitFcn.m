% osx_joystick block InitFcn callback helper function
% This function should not be called directly
function osx_joystick_InitFcn( blk )
disp('osx_joystick_InitFcn called');

% ASSUMPTION: UserData has been validated by LoadFcn
ud = get_param( blk, 'UserData' );
vals = get_param( blk, 'MaskValues' );

% Make sure the value in the 
if isempty(ud.list) || ud.SelectedJoystick>size(ud.list,1) || ud.SelectedJoystick<0
  vals{1} = '0: None';
  warning('osx_joystick:BadValue','Bad value for ''SelectedJoystick'', reverting to the ''None'' joystick');
  set_param( blk, 'MaskValues', vals );
end

% Make sure the Joystick is available. The best I can be bothered doing at
% the moment is matching ProductKey and LocationKey
list = osx_joystick_get_available();
found = 0;
for ii=1:size(list,1)
  list{ii,1} = regexprep( list{ii,1}, '\|', '-' );
  if list{ii,2}==ud.list{ud.SelectedValue,2} && regexp( list{ii,1}, ['^\d+: ',ud.list{ud.SelectedValue,2}] )
    found = 1;
    break;
  end
end
if ~found
  warning('osx_joystick:NotFound','Selected Joystick ''%s'' was not found. Reverting to the ''None'' joystick.', ud.list{ ud.selectedValue, 1 } );
  vals{1} = '0: None';
  set_param( blk, 'MaskValues', vals );
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