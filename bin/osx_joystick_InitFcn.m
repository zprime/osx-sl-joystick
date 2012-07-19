% osx_joystick block InitFcn callback helper function
% This function should not be called directly
function osx_joystick_InitFcn( blk )
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
if ud.SelectedJoystick ~= 0
  list = osx_joystick_get_available();
  foundIdentical = 0;
  foundPartial = 0;
  for ii=1:size(list,1)
    list{ii,1} = regexprep( list{ii,1}, '\|', '-' );
    if ~isempty( regexp( ud.list{ ud.SelectedJoystick ,1}, [ '^\d+: ', list{ ii, 1 }], 'once' ) )
      foundPartial = ii;
      if (list{ii,2}==ud.list{ud.SelectedJoystick,2})
        foundIdentical = ii;
        break;
      end
    end
  end
  if foundIdentical
    for ii=1:size(list,1)
      list{ii,1} = sprintf('%i: %s',ii,list{ii,1});
    end
    ud.list = list;
    vals{1} = list{foundIdentical,1};
    str = 'popup(0: None),checkbox,checkbox,checkbox,checkbox,edit';
    ud.SelectedJoystick = foundIdentical;
    ud.MaskStyleString = regexprep( str, 'popup\(.*\),checkbox,checkbox,checkbox,checkbox,edit', ['popup(0: None', sprintf([char(124),'%s'],ud.list{:,1}),'),checkbox,checkbox,checkbox,checkbox,edit'], 'once' );
  elseif foundPartial
    joyProdKey = regexp( ud.list{ ud.SelectedJoystick, 1 }, '^\d+: (.*)', 'tokens' );
    joyProdKey = joyProdKey{1};
    warning('osx_joystick:FoundReplacement','Selected Joystick ''%s'' @ 0x%X was not found. Substituting with ''%s'' @ 0x%X instead.',joyProdKey{1},ud.list{ud.SelectedJoystick,2},joyProdKey{1},list{foundPartial,2});
    for ii=1:size(list,1)
      list{ii,1} = sprintf('%i: %s',ii,list{ii,1});
    end
    ud.list = list;
    vals{1} = list{foundPartial,1};
    str = 'popup(0: None),checkbox,checkbox,checkbox,checkbox,edit';
    ud.SelectedJoystick = foundPartial;
    ud.MaskStyleString = regexprep( str, 'popup\(.*\),checkbox,checkbox,checkbox,checkbox,edit', ['popup(0: None', sprintf([char(124),'%s'],ud.list{:,1}),'),checkbox,checkbox,checkbox,checkbox,edit'], 'once' );
  else
    joyProdKey = regexp( ud.list{ ud.SelectedJoystick, 1 }, '^\d+: (.*)', 'tokens' );
    joyProdKey = joyProdKey{1};
    warning('osx_joystick:NotFound','Selected Joystick ''%s'' @ 0x%X was not found. Reverting to the ''None'' joystick.', joyProdKey{1}, ud.list{ ud.SelectedJoystick, 2 } );
    vals{1} = '0: None';
    ud.MaskStyleString = 'popup(0: None),checkbox,checkbox,checkbox,checkbox,edit';
    ud.SelectedJoystick = 0;
  end
  set_param( blk, 'MaskStyleString', ud.MaskStyleString, 'MaskValues', vals, 'UserData', ud );
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