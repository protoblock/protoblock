.pragma library
.import Protobuf 1.0 as Protobuf
'use strict';

var _file = {
  get descriptor() {
    if(!this._desc) {
      this._desc = Protobuf.DescriptorPool.addFileDescriptor('Cg1BcGlEYXRhLnByb3RvEgpmYW50YXN5Yml0Ii0KD0ZhbnRhc3lOYW1lSGFzaBIMCgRuYW1lGAogASgJEgwKBGhhc2gYHiABKAQiowEKDERpc3RyaWJ1dGlvbhIOCgZnYW1laWQYAiABKAkSDgoGdGVhbWlkGAQgASgJEg4KBnNlYXNvbhgFIAEoBRIMCgR3ZWVrGAYgASgFEhYKDmZhbnRhc3lfbmFtZWlkGAEgASgEEhAKCHBsYXllcmlkGAMgASgJEgwKBHByb2oYByABKAUSDQoFYXdhcmQYCCABKAISDgoGcmVzdWx0GAkgASgCIlAKClByb2pLc3RhdHMSCgoCcGEYCiABKAISDAoEZmczMBgUIAEoAhIMCgRmZzQwGB4gASgCEgwKBGZnNTAYKCABKAISDAoEZmc2MBgyIAEoAiJ7CgpQcm9qRHN0YXRzEg0KBXR3b3B0GGQgASgCEg0KBW9uZXB0GG4gASgCEg0KBWRlZnRkGHggASgCEg0KBHNmdHkYggEgASgCEhIKCXR1cm5vdmVycxiMASABKAISDgoFc2Fja3MYlgEgASgCEg0KBHB0c2EYoAEgASgCIrYBCgpQcm9qT3N0YXRzEg8KB3Bhc3N5ZHMYCiABKAISDgoGcGFzc3RkGBQgASgCEg8KB3J1c2h5ZHMYHiABKAISDgoGcnVzaHRkGCggASgCEg4KBnJlY3lkcxgyIAEoAhINCgVyZWN0ZBg8IAEoAhILCgNyZWMYRiABKAISDAoEcGludBhQIAEoAhIOCgZmdW1ibGUYWiABKAISDQoFdHdvcHQYZCABKAISDQoFb25lcHQYbiABKAIigwEKCVByb2pTdGF0cxImCgZvc3RhdHMYCiABKAsyFi5mYW50YXN5Yml0LlByb2pPc3RhdHMSJgoGZHN0YXRzGBQgASgLMhYuZmFudGFzeWJpdC5Qcm9qRHN0YXRzEiYKBmtzdGF0cxgeIAEoCzIWLmZhbnRhc3liaXQuUHJvaktzdGF0cw==', 766);
    }
    if (!this._desc)
      console.warn('Failed to initialize: ApiData.proto');
    return this._desc;
  },
};

var FantasyNameHash = (function() {
  var type = function(values) {
    this._fields = new Array(2);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof FantasyNameHash) {
      this._mergeFromRawArray(values._raw);
    } else if (values instanceof Array) {
      this._mergeFromRawArray(values);
    } else {
      for (var k in values) {
        if (this[k] instanceof Function) {
          this[k](values[k]);
        } else {
          this[k] = values[k];
        }
      }
    }
  };

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(0));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setName(rawArray[0][0]);
      this.setHash(rawArray[0][1]);
    }
  };

  type.prototype.getName = function() {
    return typeof this._fields[0] == 'undefined' ? '' : this._fields[0];
  }
  type.prototype.setName = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearName = function() {
    this._fields[0] = '';
  };
  Object.defineProperty(type.prototype, 'name', {
    get: type.prototype.getName,
    set: type.prototype.setName,
  });

  type.prototype.getHash = function() {
    return typeof this._fields[1] == 'undefined' ? 0 : this._fields[1];
  }
  type.prototype.setHash = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearHash = function() {
    this._fields[1] = 0;
  };
  Object.defineProperty(type.prototype, 'hash', {
    get: type.prototype.getHash,
    set: type.prototype.setHash,
  });

  return type;
})();

var Distribution = (function() {
  var type = function(values) {
    this._fields = new Array(9);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof Distribution) {
      this._mergeFromRawArray(values._raw);
    } else if (values instanceof Array) {
      this._mergeFromRawArray(values);
    } else {
      for (var k in values) {
        if (this[k] instanceof Function) {
          this[k](values[k]);
        } else {
          this[k] = values[k];
        }
      }
    }
  };

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(1));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setGameid(rawArray[0][0]);
      this.setTeamid(rawArray[0][1]);
      this.setSeason(rawArray[0][2]);
      this.setWeek(rawArray[0][3]);
      this.setFantasyNameid(rawArray[0][4]);
      this.setPlayerid(rawArray[0][5]);
      this.setProj(rawArray[0][6]);
      this.setAward(rawArray[0][7]);
      this.setResult(rawArray[0][8]);
    }
  };

  type.prototype.getGameid = function() {
    return typeof this._fields[0] == 'undefined' ? '' : this._fields[0];
  }
  type.prototype.setGameid = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearGameid = function() {
    this._fields[0] = '';
  };
  Object.defineProperty(type.prototype, 'gameid', {
    get: type.prototype.getGameid,
    set: type.prototype.setGameid,
  });

  type.prototype.getTeamid = function() {
    return typeof this._fields[1] == 'undefined' ? '' : this._fields[1];
  }
  type.prototype.setTeamid = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearTeamid = function() {
    this._fields[1] = '';
  };
  Object.defineProperty(type.prototype, 'teamid', {
    get: type.prototype.getTeamid,
    set: type.prototype.setTeamid,
  });

  type.prototype.getSeason = function() {
    return typeof this._fields[2] == 'undefined' ? 0 : this._fields[2];
  }
  type.prototype.setSeason = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearSeason = function() {
    this._fields[2] = 0;
  };
  Object.defineProperty(type.prototype, 'season', {
    get: type.prototype.getSeason,
    set: type.prototype.setSeason,
  });

  type.prototype.getWeek = function() {
    return typeof this._fields[3] == 'undefined' ? 0 : this._fields[3];
  }
  type.prototype.setWeek = function(value) {
      this._fields[3] = value;
  };
  type.prototype.clearWeek = function() {
    this._fields[3] = 0;
  };
  Object.defineProperty(type.prototype, 'week', {
    get: type.prototype.getWeek,
    set: type.prototype.setWeek,
  });

  type.prototype.getFantasyNameid = function() {
    return typeof this._fields[4] == 'undefined' ? 0 : this._fields[4];
  }
  type.prototype.setFantasyNameid = function(value) {
      this._fields[4] = value;
  };
  type.prototype.clearFantasyNameid = function() {
    this._fields[4] = 0;
  };
  Object.defineProperty(type.prototype, 'fantasyNameid', {
    get: type.prototype.getFantasyNameid,
    set: type.prototype.setFantasyNameid,
  });

  type.prototype.getPlayerid = function() {
    return typeof this._fields[5] == 'undefined' ? '' : this._fields[5];
  }
  type.prototype.setPlayerid = function(value) {
      this._fields[5] = value;
  };
  type.prototype.clearPlayerid = function() {
    this._fields[5] = '';
  };
  Object.defineProperty(type.prototype, 'playerid', {
    get: type.prototype.getPlayerid,
    set: type.prototype.setPlayerid,
  });

  type.prototype.getProj = function() {
    return typeof this._fields[6] == 'undefined' ? 0 : this._fields[6];
  }
  type.prototype.setProj = function(value) {
      this._fields[6] = value;
  };
  type.prototype.clearProj = function() {
    this._fields[6] = 0;
  };
  Object.defineProperty(type.prototype, 'proj', {
    get: type.prototype.getProj,
    set: type.prototype.setProj,
  });

  type.prototype.getAward = function() {
    return typeof this._fields[7] == 'undefined' ? 0.000000 : this._fields[7];
  }
  type.prototype.setAward = function(value) {
      this._fields[7] = value;
  };
  type.prototype.clearAward = function() {
    this._fields[7] = 0.000000;
  };
  Object.defineProperty(type.prototype, 'award', {
    get: type.prototype.getAward,
    set: type.prototype.setAward,
  });

  type.prototype.getResult = function() {
    return typeof this._fields[8] == 'undefined' ? 0.000000 : this._fields[8];
  }
  type.prototype.setResult = function(value) {
      this._fields[8] = value;
  };
  type.prototype.clearResult = function() {
    this._fields[8] = 0.000000;
  };
  Object.defineProperty(type.prototype, 'result', {
    get: type.prototype.getResult,
    set: type.prototype.setResult,
  });

  return type;
})();

var ProjKstats = (function() {
  var type = function(values) {
    this._fields = new Array(5);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof ProjKstats) {
      this._mergeFromRawArray(values._raw);
    } else if (values instanceof Array) {
      this._mergeFromRawArray(values);
    } else {
      for (var k in values) {
        if (this[k] instanceof Function) {
          this[k](values[k]);
        } else {
          this[k] = values[k];
        }
      }
    }
  };

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(2));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setPa(rawArray[0][0]);
      this.setFg30(rawArray[0][1]);
      this.setFg40(rawArray[0][2]);
      this.setFg50(rawArray[0][3]);
      this.setFg60(rawArray[0][4]);
    }
  };

  type.prototype.getPa = function() {
    return typeof this._fields[0] == 'undefined' ? 0.000000 : this._fields[0];
  }
  type.prototype.setPa = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearPa = function() {
    this._fields[0] = 0.000000;
  };
  Object.defineProperty(type.prototype, 'pa', {
    get: type.prototype.getPa,
    set: type.prototype.setPa,
  });

  type.prototype.getFg30 = function() {
    return typeof this._fields[1] == 'undefined' ? 0.000000 : this._fields[1];
  }
  type.prototype.setFg30 = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearFg30 = function() {
    this._fields[1] = 0.000000;
  };
  Object.defineProperty(type.prototype, 'fg30', {
    get: type.prototype.getFg30,
    set: type.prototype.setFg30,
  });

  type.prototype.getFg40 = function() {
    return typeof this._fields[2] == 'undefined' ? 0.000000 : this._fields[2];
  }
  type.prototype.setFg40 = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearFg40 = function() {
    this._fields[2] = 0.000000;
  };
  Object.defineProperty(type.prototype, 'fg40', {
    get: type.prototype.getFg40,
    set: type.prototype.setFg40,
  });

  type.prototype.getFg50 = function() {
    return typeof this._fields[3] == 'undefined' ? 0.000000 : this._fields[3];
  }
  type.prototype.setFg50 = function(value) {
      this._fields[3] = value;
  };
  type.prototype.clearFg50 = function() {
    this._fields[3] = 0.000000;
  };
  Object.defineProperty(type.prototype, 'fg50', {
    get: type.prototype.getFg50,
    set: type.prototype.setFg50,
  });

  type.prototype.getFg60 = function() {
    return typeof this._fields[4] == 'undefined' ? 0.000000 : this._fields[4];
  }
  type.prototype.setFg60 = function(value) {
      this._fields[4] = value;
  };
  type.prototype.clearFg60 = function() {
    this._fields[4] = 0.000000;
  };
  Object.defineProperty(type.prototype, 'fg60', {
    get: type.prototype.getFg60,
    set: type.prototype.setFg60,
  });

  return type;
})();

var ProjDstats = (function() {
  var type = function(values) {
    this._fields = new Array(7);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof ProjDstats) {
      this._mergeFromRawArray(values._raw);
    } else if (values instanceof Array) {
      this._mergeFromRawArray(values);
    } else {
      for (var k in values) {
        if (this[k] instanceof Function) {
          this[k](values[k]);
        } else {
          this[k] = values[k];
        }
      }
    }
  };

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(3));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setTwopt(rawArray[0][0]);
      this.setOnept(rawArray[0][1]);
      this.setDeftd(rawArray[0][2]);
      this.setSfty(rawArray[0][3]);
      this.setTurnovers(rawArray[0][4]);
      this.setSacks(rawArray[0][5]);
      this.setPtsa(rawArray[0][6]);
    }
  };

  type.prototype.getTwopt = function() {
    return typeof this._fields[0] == 'undefined' ? 0.000000 : this._fields[0];
  }
  type.prototype.setTwopt = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearTwopt = function() {
    this._fields[0] = 0.000000;
  };
  Object.defineProperty(type.prototype, 'twopt', {
    get: type.prototype.getTwopt,
    set: type.prototype.setTwopt,
  });

  type.prototype.getOnept = function() {
    return typeof this._fields[1] == 'undefined' ? 0.000000 : this._fields[1];
  }
  type.prototype.setOnept = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearOnept = function() {
    this._fields[1] = 0.000000;
  };
  Object.defineProperty(type.prototype, 'onept', {
    get: type.prototype.getOnept,
    set: type.prototype.setOnept,
  });

  type.prototype.getDeftd = function() {
    return typeof this._fields[2] == 'undefined' ? 0.000000 : this._fields[2];
  }
  type.prototype.setDeftd = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearDeftd = function() {
    this._fields[2] = 0.000000;
  };
  Object.defineProperty(type.prototype, 'deftd', {
    get: type.prototype.getDeftd,
    set: type.prototype.setDeftd,
  });

  type.prototype.getSfty = function() {
    return typeof this._fields[3] == 'undefined' ? 0.000000 : this._fields[3];
  }
  type.prototype.setSfty = function(value) {
      this._fields[3] = value;
  };
  type.prototype.clearSfty = function() {
    this._fields[3] = 0.000000;
  };
  Object.defineProperty(type.prototype, 'sfty', {
    get: type.prototype.getSfty,
    set: type.prototype.setSfty,
  });

  type.prototype.getTurnovers = function() {
    return typeof this._fields[4] == 'undefined' ? 0.000000 : this._fields[4];
  }
  type.prototype.setTurnovers = function(value) {
      this._fields[4] = value;
  };
  type.prototype.clearTurnovers = function() {
    this._fields[4] = 0.000000;
  };
  Object.defineProperty(type.prototype, 'turnovers', {
    get: type.prototype.getTurnovers,
    set: type.prototype.setTurnovers,
  });

  type.prototype.getSacks = function() {
    return typeof this._fields[5] == 'undefined' ? 0.000000 : this._fields[5];
  }
  type.prototype.setSacks = function(value) {
      this._fields[5] = value;
  };
  type.prototype.clearSacks = function() {
    this._fields[5] = 0.000000;
  };
  Object.defineProperty(type.prototype, 'sacks', {
    get: type.prototype.getSacks,
    set: type.prototype.setSacks,
  });

  type.prototype.getPtsa = function() {
    return typeof this._fields[6] == 'undefined' ? 0.000000 : this._fields[6];
  }
  type.prototype.setPtsa = function(value) {
      this._fields[6] = value;
  };
  type.prototype.clearPtsa = function() {
    this._fields[6] = 0.000000;
  };
  Object.defineProperty(type.prototype, 'ptsa', {
    get: type.prototype.getPtsa,
    set: type.prototype.setPtsa,
  });

  return type;
})();

var ProjOstats = (function() {
  var type = function(values) {
    this._fields = new Array(11);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof ProjOstats) {
      this._mergeFromRawArray(values._raw);
    } else if (values instanceof Array) {
      this._mergeFromRawArray(values);
    } else {
      for (var k in values) {
        if (this[k] instanceof Function) {
          this[k](values[k]);
        } else {
          this[k] = values[k];
        }
      }
    }
  };

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(4));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setPassyds(rawArray[0][0]);
      this.setPasstd(rawArray[0][1]);
      this.setRushyds(rawArray[0][2]);
      this.setRushtd(rawArray[0][3]);
      this.setRecyds(rawArray[0][4]);
      this.setRectd(rawArray[0][5]);
      this.setRec(rawArray[0][6]);
      this.setPint(rawArray[0][7]);
      this.setFumble(rawArray[0][8]);
      this.setTwopt(rawArray[0][9]);
      this.setOnept(rawArray[0][10]);
    }
  };

  type.prototype.getPassyds = function() {
    return typeof this._fields[0] == 'undefined' ? 0.000000 : this._fields[0];
  }
  type.prototype.setPassyds = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearPassyds = function() {
    this._fields[0] = 0.000000;
  };
  Object.defineProperty(type.prototype, 'passyds', {
    get: type.prototype.getPassyds,
    set: type.prototype.setPassyds,
  });

  type.prototype.getPasstd = function() {
    return typeof this._fields[1] == 'undefined' ? 0.000000 : this._fields[1];
  }
  type.prototype.setPasstd = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearPasstd = function() {
    this._fields[1] = 0.000000;
  };
  Object.defineProperty(type.prototype, 'passtd', {
    get: type.prototype.getPasstd,
    set: type.prototype.setPasstd,
  });

  type.prototype.getRushyds = function() {
    return typeof this._fields[2] == 'undefined' ? 0.000000 : this._fields[2];
  }
  type.prototype.setRushyds = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearRushyds = function() {
    this._fields[2] = 0.000000;
  };
  Object.defineProperty(type.prototype, 'rushyds', {
    get: type.prototype.getRushyds,
    set: type.prototype.setRushyds,
  });

  type.prototype.getRushtd = function() {
    return typeof this._fields[3] == 'undefined' ? 0.000000 : this._fields[3];
  }
  type.prototype.setRushtd = function(value) {
      this._fields[3] = value;
  };
  type.prototype.clearRushtd = function() {
    this._fields[3] = 0.000000;
  };
  Object.defineProperty(type.prototype, 'rushtd', {
    get: type.prototype.getRushtd,
    set: type.prototype.setRushtd,
  });

  type.prototype.getRecyds = function() {
    return typeof this._fields[4] == 'undefined' ? 0.000000 : this._fields[4];
  }
  type.prototype.setRecyds = function(value) {
      this._fields[4] = value;
  };
  type.prototype.clearRecyds = function() {
    this._fields[4] = 0.000000;
  };
  Object.defineProperty(type.prototype, 'recyds', {
    get: type.prototype.getRecyds,
    set: type.prototype.setRecyds,
  });

  type.prototype.getRectd = function() {
    return typeof this._fields[5] == 'undefined' ? 0.000000 : this._fields[5];
  }
  type.prototype.setRectd = function(value) {
      this._fields[5] = value;
  };
  type.prototype.clearRectd = function() {
    this._fields[5] = 0.000000;
  };
  Object.defineProperty(type.prototype, 'rectd', {
    get: type.prototype.getRectd,
    set: type.prototype.setRectd,
  });

  type.prototype.getRec = function() {
    return typeof this._fields[6] == 'undefined' ? 0.000000 : this._fields[6];
  }
  type.prototype.setRec = function(value) {
      this._fields[6] = value;
  };
  type.prototype.clearRec = function() {
    this._fields[6] = 0.000000;
  };
  Object.defineProperty(type.prototype, 'rec', {
    get: type.prototype.getRec,
    set: type.prototype.setRec,
  });

  type.prototype.getPint = function() {
    return typeof this._fields[7] == 'undefined' ? 0.000000 : this._fields[7];
  }
  type.prototype.setPint = function(value) {
      this._fields[7] = value;
  };
  type.prototype.clearPint = function() {
    this._fields[7] = 0.000000;
  };
  Object.defineProperty(type.prototype, 'pint', {
    get: type.prototype.getPint,
    set: type.prototype.setPint,
  });

  type.prototype.getFumble = function() {
    return typeof this._fields[8] == 'undefined' ? 0.000000 : this._fields[8];
  }
  type.prototype.setFumble = function(value) {
      this._fields[8] = value;
  };
  type.prototype.clearFumble = function() {
    this._fields[8] = 0.000000;
  };
  Object.defineProperty(type.prototype, 'fumble', {
    get: type.prototype.getFumble,
    set: type.prototype.setFumble,
  });

  type.prototype.getTwopt = function() {
    return typeof this._fields[9] == 'undefined' ? 0.000000 : this._fields[9];
  }
  type.prototype.setTwopt = function(value) {
      this._fields[9] = value;
  };
  type.prototype.clearTwopt = function() {
    this._fields[9] = 0.000000;
  };
  Object.defineProperty(type.prototype, 'twopt', {
    get: type.prototype.getTwopt,
    set: type.prototype.setTwopt,
  });

  type.prototype.getOnept = function() {
    return typeof this._fields[10] == 'undefined' ? 0.000000 : this._fields[10];
  }
  type.prototype.setOnept = function(value) {
      this._fields[10] = value;
  };
  type.prototype.clearOnept = function() {
    this._fields[10] = 0.000000;
  };
  Object.defineProperty(type.prototype, 'onept', {
    get: type.prototype.getOnept,
    set: type.prototype.setOnept,
  });

  return type;
})();

var ProjStats = (function() {
  var type = function(values) {
    this._fields = new Array(3);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof ProjStats) {
      this._mergeFromRawArray(values._raw);
    } else if (values instanceof Array) {
      this._mergeFromRawArray(values);
    } else {
      for (var k in values) {
        if (this[k] instanceof Function) {
          this[k](values[k]);
        } else {
          this[k] = values[k];
        }
      }
    }
  };

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(5));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      if (typeof this.ostats == 'undefined') {
        this.setOstats({});
      }
      this._ostats._mergeFromRawArray(rawArray[0][0]);
      if (typeof this.dstats == 'undefined') {
        this.setDstats({});
      }
      this._dstats._mergeFromRawArray(rawArray[0][1]);
      if (typeof this.kstats == 'undefined') {
        this.setKstats({});
      }
      this._kstats._mergeFromRawArray(rawArray[0][2]);
    }
  };

  type.prototype.getOstats = function() {
    return this._ostats;
  }
  type.prototype.setOstats = function(value) {
    var msg = this._maybeConvertToMessage(ProjOstats, value);
    this._ostats = msg;
    this._fields[0] = msg._raw;
  };
  type.prototype.clearOstats = function() {
    this._fields[0] = undefined;
    this._ostats = undefined;
  };
  Object.defineProperty(type.prototype, 'ostats', {
    get: type.prototype.getOstats,
    set: type.prototype.setOstats,
  });

  type.prototype.getDstats = function() {
    return this._dstats;
  }
  type.prototype.setDstats = function(value) {
    var msg = this._maybeConvertToMessage(ProjDstats, value);
    this._dstats = msg;
    this._fields[1] = msg._raw;
  };
  type.prototype.clearDstats = function() {
    this._fields[1] = undefined;
    this._dstats = undefined;
  };
  Object.defineProperty(type.prototype, 'dstats', {
    get: type.prototype.getDstats,
    set: type.prototype.setDstats,
  });

  type.prototype.getKstats = function() {
    return this._kstats;
  }
  type.prototype.setKstats = function(value) {
    var msg = this._maybeConvertToMessage(ProjKstats, value);
    this._kstats = msg;
    this._fields[2] = msg._raw;
  };
  type.prototype.clearKstats = function() {
    this._fields[2] = undefined;
    this._kstats = undefined;
  };
  Object.defineProperty(type.prototype, 'kstats', {
    get: type.prototype.getKstats,
    set: type.prototype.setKstats,
  });

  return type;
})();

