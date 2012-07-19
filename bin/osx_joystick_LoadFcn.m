function osx_joystick_LoadFcn( blk )
% Restore the block using the saved UserData

% Check that this machine is a mac
if ~ismac
  warning('osx_joystick:NotOSX','Surprisingly, the OSX Joystick block only works if you are using Matlab on OSX. /sarcasm');
end

% Get and Validate UserData
ud = get_param( blk, 'UserData' );
if isempty(ud) || ~isstruct( ud ) || ~all( isfield( ud, {'list','SelectedJoystick','sizes','MaskStyleString'} ) )
  % If UserData does not exist, create some
  clear('ud');
  ud.list = {}; % List of available joysticks.
  ud.SelectedJoystick = 0; % Index to the Joystick, 0=None
  ud.sizes = [0 0 0 0]; % Size information about the current joystick
  ud.MaskStyleString = 'popup(0: None),checkbox,checkbox,checkbox,checkbox,edit';
  set_param( blk, 'UserData', ud );
  set_param( blk, 'UserDataPersistent', 'on' );
end

% Restore Mask
set_param( blk, 'MaskStyleString', ud.MaskStyleString );

% Restore Value
vals = get_param( blk, 'MaskValues' );
if ud.SelectedJoystick <= 0 || ud.SelectedJoystick > size(ud.list,1)
  vals{1} = '0: None';
  ud.SelectedJoystick = 0;
else
  vals{1} = ud.list{ ud.SelectedJoystick, 1 };
end
set_param( blk, 'MaskValues', vals );

% MaskInitFcn will now redraw the mask appropriately.

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