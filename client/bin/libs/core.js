// Generated by CoffeeScript 1.6.3
(function() {
  var cancelNotification, host, hostJSON, loginSuccess, showNotification, successJSON, updateNames,
    _this = this,
    __indexOf = [].indexOf || function(item) { for (var i = 0, l = this.length; i < l; i++) { if (i in this && this[i] === item) return i; } return -1; };

  this.setCookie = function(name, value, days) {
    var date, expires;
    if (days) {
      date = new Date();
      date.setTime(date.getTime() + days * 24 * 60 * 60 * 1000);
      expires = "; expires=" + (date.toGMTString());
    } else {
      expires = "";
    }
    return document.cookie = name + "=" + value + expires + "; path=/";
  };

  this.getCookie = function(name) {
    var cookie, cookies, eq, raw_cookie, _i, _len;
    cookies = document.cookie.split(';');
    for (_i = 0, _len = cookies.length; _i < _len; _i++) {
      raw_cookie = cookies[_i];
      cookie = $.trim(raw_cookie);
      eq = cookie.indexOf('=');
      if (cookie.slice(0, eq) === name) {
        return cookie.slice(eq + 1);
      }
    }
    return null;
  };

  this.deleteCookie = function(name) {
    return setCookie(name, "", -1);
  };

  if (this.callback == null) {
    this.callback = [];
  }

  if (this.game == null) {
    this.game = [];
  }

  this.callback.startGame = function(data) {
    _this.loadContent("game.html");
    _this.game.accountVal = data.player.account;
    _this.game.bidVal = 0;
    _this.game.renderPlayer(data.player);
    _this.game.renderState(data.others);
    return game.setCards(data.player.cards);
  };

  this.callback.refreshState = function(data) {
    return _this.game.refreshState(data);
  };

  this.callback.renderPlayer = function(data) {
    return game.renderPlayer(data);
  };

  this.callback.finishGame = function(data) {
    return _this.game.finishGame(data);
  };

  this.game.refreshState = function(data) {
    game.render(data.others);
    return setTimeout(game.update, 1000);
  };

  this.game.finishGame = function(data) {
    if (data.won === true) {
      return notify("You win!", 3000);
    } else {
      return notify("You loose!", 3000, notify.warning);
    }
  };

  this.game.username = this.username;

  this.game.renderPlayer = function(data) {
    var div;
    if (data != null) {
      if (data.bid != null) {
        this.bidVal = data.bid;
      }
      if (data.account != null) {
        this.accountVal = data.account;
      }
    }
    div = "	<div>" + this.username + ":</div>	<div> Account: " + this.accountVal + "</div>	<div> Bid: " + this.bidVal + "</div>	";
    return $('#player-state').html(div);
  };

  game.renderState = function(data) {
    var div, player;
    div = (function() {
      var _i, _len, _results;
      _results = [];
      for (_i = 0, _len = data.length; _i < _len; _i++) {
        player = data[_i];
        _results.push("<li> " + player.name + "($" + player.account + ") current bid: $" + player.bid + " and player " + player.state + " </li>");
      }
      return _results;
    })();
    div.join(" ");
    return $("#game-state").replaceWith('<ul class="list">' + div + '</ul>');
  };

  game.update = function() {
    return load('updateGame');
  };

  game.bid = function() {
    var newBid, parameters;
    newBid = $('input[name=""]').val();
    parameters = [
      {
        bid: newBid
      }
    ];
    return load('bid', parameters);
  };

  game.pass = function() {
    load('pass', null);
    return $('.control').attr('disabled', true);
  };

  game.setCards = function(data) {
    var card, cards, i, x, _i, _len;
    game.cards = {};
    i = 0;
    for (_i = 0, _len = data.length; _i < _len; _i++) {
      x = data[_i];
      game.cards[++i] = x;
    }
    game.replaced = [];
    i = 0;
    cards = (function() {
      var _j, _len1, _results;
      _results = [];
      for (_j = 0, _len1 = data.length; _j < _len1; _j++) {
        card = data[_j];
        _results.push("<div class=\"card-border\" id=\"card-border-" + (++i) + "\" onClick=\"game.toggleCardReplace(" + i + ")\">		<div class=\"card\" id=\"card-" + i + "\" onDrop=\"drop(event)\" onDragOver=\"dragOver(event)\">		<img id=\"image-" + i + "\" draggable=\"true\" onDragStart=\"drag(event)\" src=\"/cards/" + card + ".png\">		</div>		</div>");
      }
      return _results;
    })();
    cards.join(' ');
    return $('#cards').html(cards);
  };

  game.change = function() {
    var parameters, x;
    parameters = [
      {
        replaced: [
          (function() {
            var _i, _len, _ref, _results;
            _ref = this.replaced;
            _results = [];
            for (_i = 0, _len = _ref.length; _i < _len; _i++) {
              x = _ref[_i];
              _results.push(this.cards[x]);
            }
            return _results;
          }).call(this)
        ]
      }
    ];
    return load('replaceCards', parameters);
  };

  game.toggleCardReplace = function(i) {
    if (__indexOf.call(game.replaced, i) < 0) {
      game.replaced.push(i);
      return $("#card-" + i).addClass('card-replace');
    } else {
      game.replaced = game.replaced.filter(function(id) {
        return id !== i;
      });
      return $("#card-" + i).removeClass('card-replace');
    }
  };

  host = "http://localhost:3000/";

  hostJSON = host + 'test';

  this.loadContent = function(url, parameters) {
    if (parameters == null) {
      parameters = null;
    }
    return $.ajax({
      type: 'GET',
      url: host + url,
      data: parameters,
      success: function(data) {
        return $("#content").replaceWith(data);
      },
      error: function(_, __, text) {
        return notify("Error while connecting to the server", 2500, notify.error);
      },
      async: false
    });
  };

  this.load = function(method, requestParameters) {
    var parameters;
    parameters = [
      {
        username: username,
        method: method,
        parameters: requestParameters
      }
    ];
    return $.ajax({
      type: "POST",
      url: hostJSON,
      data: JSON.stringify(parameters),
      success: successJSON,
      error: function(_, __, text) {
        return notify("Error while connecting to server", 2500, notify.error);
      },
      timeout: 1000
    });
  };

  if (this.callback == null) {
    this.callback = [];
  }

  successJSON = function(data) {
    var method, parameters;
    if (data.error != null) {
      notify(data.error, 2500, notify.error);
      return false;
    }
    method = data.method;
    parameters = data.parameters;
    if (method in _this.callback) {
      return _this.callback[method](parameters);
    } else {
      notify("Unknown answer from server.", 2500, notify.error);
      return false;
    }
  };

  updateNames = function() {
    return load('updateNames', []);
  };

  if (this.callback == null) {
    this.callback = [];
  }

  this.callback.refreshNames = function(data) {
    var name, nameList, names;
    nameList = (function() {
      var _i, _len, _ref, _results;
      _ref = data.names;
      _results = [];
      for (_i = 0, _len = _ref.length; _i < _len; _i++) {
        name = _ref[_i];
        _results.push("<li> " + name + " - Ready </li>");
      }
      return _results;
    })();
    names = namesList.join(' ');
    $("#player-list").html(names);
    return setTimeout(updateNames, 1000);
  };

  this.logIn = function() {
    var login, parameters;
    login = $('input[name=login]').val();
    if ($('input[name="remember-me"]').prop('checked')) {
      setCookie("username", login, 60);
    }
    parameters = [
      {
        "login": login
      }
    ];
    load('login', parameters, loginSuccess);
    return false;
  };

  loginSuccess = function(data) {
    this.username = data.response;
    loadContent("/lobby.html");
    return refreshNames();
  };

  $(function() {
    var login;
    _this.username = null;
    login = getCookie("username");
    if (login != null) {
      $('input[name="login"]').val(login);
      $('input[name="remember-me"]').prop('checked', true);
    } else {
      notify("<h1>Welcome</h1><div>Please sign in</div>", 3000);
    }
    return $('#form-signin').submit(logIn);
  });

  showNotification = function() {
    var notification;
    notification = $("#notification");
    notification.is('.fly-over');
    return notification.addClass('in');
  };

  cancelNotification = function() {
    var notification;
    notification = $("#notification");
    notification.removeClass('in');
    if (notification.hasClass('error')) {
      setTimeout((function() {
        return notification.removeClass('error');
      }), 200);
    }
    if (notification.hasClass('warning')) {
      return setTimeout((function() {
        return notification.removeClass('warning');
      }), 200);
    }
  };

  this.notify = function(message, time, state) {
    var notification;
    if (state == null) {
      state = notify.normal;
    }
    notification = $("#notification");
    switch (state) {
      case notify.error:
        notification.addClass('error');
        break;
      case notify.warning:
        notification.addClass('warning');
    }
    notification.html(message);
    setTimeout(showNotification, 200);
    return setTimeout(cancelNotification, 200 + time);
  };

  this.notify.normal = 0;

  this.notify.warning = 1;

  this.notify.error = 2;

  $(function() {
    var ntf;
    ntf = $('<div>');
    ntf.attr('id', 'notification');
    ntf.html('<h2>Welcome</h2><div>Please sign in</div>');
    ntf.addClass('jumbotron');
    ntf.addClass('flyover');
    ntf.addClass('flyover-centered');
    return ntf.appendTo($('body'));
  });

  this.drag = function(event) {
    var i, _i, _results;
    event.dataTransfer.setData("Text", event.target.id);
    game.replaced = [];
    _results = [];
    for (i = _i = 1; _i <= 5; i = ++_i) {
      _results.push($("#card-" + i).removeClass('card-replace'));
    }
    return _results;
  };

  this.drop = function(event) {
    var i, source, sourceId, sourceSRC, target, targetId, targetSRC;
    event.preventDefault();
    sourceId = event.dataTransfer.getData("Text");
    targetId = event.target.id;
    source = sourceId.split('-');
    target = targetId.split('-');
    if (source[0] === 'image' && target[0] === 'image') {
      sourceSRC = $('#' + sourceId).attr('src');
      targetSRC = $('#' + targetId).attr('src');
      $('#' + sourceId).attr('src', targetSRC);
      $('#' + targetId).attr('src', sourceSRC);
      i = _this.game.cards[target[1]];
      _this.game.cards[target[1]] = _this.game.cards[source[1]];
      return _this.game.cards[source[1]] = i;
    }
  };

  this.dragOver = function(event) {
    return event.preventDefault();
  };

  this.dragEnter = function(event) {
    return event.preventDefault();
  };

}).call(this);
